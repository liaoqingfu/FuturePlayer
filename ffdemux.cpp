#include "ffdemux.h"
#include "formatcontext.h"
#include "demuxerfactory.hpp"

#include <qDebug>

class AVPacketRAII
{
public:
    inline AVPacketRAII(AVPacket *packet) :
        packet(packet)
    {}
    inline ~AVPacketRAII()
    {
        av_packet_unref(packet);
    }

private:
    AVPacket *packet;
};

int write_packet(void *opaque, uint8_t *buf, int buf_size)
{
    qDebug()<< "write_packet = " << buf_size;

    return buf_size;
}

inline void FFDemux::freeCommSpace()
{
    avformat_close_input(&formatCtx_);
}

//FFDemux::FFDemux():
//    streamInfo_ (0)
//{
//    qDebug() << "FFDemux init";
//    inputFmt_ = nullptr;
//    dictOptions_ = nullptr;
//    formatCtx_ = nullptr;
//    isEof_ = false;
//#if LIBAVCODEC_VERSION_MAJOR >= 57
//    packet_ = av_packet_alloc();
//#else
//    packet_ = (AVPacket *)av_malloc(sizeof(AVPacket));
//    av_init_packet(packet_);
//#endif
//}
FFDemux::FFDemux(StreamInfo &streamInfo):
    streamInfo_ (streamInfo)
{
    qDebug() << "FFDemux init";
    inputFmt_ = nullptr;
    dictOptions_ = nullptr;
    formatCtx_ = nullptr;
    isEof_ = false;
#if LIBAVCODEC_VERSION_MAJOR >= 57
    packet_ = av_packet_alloc();
#else
    packet_ = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(packet_);
#endif
}

qint64 FFDemux::size() const
{
    qint64 bytes = -1;
    for (const FormatContext *fmtCtx : formatContexts)
    {
        const qint64 s = fmtCtx->size();
        if (s < 0)
            return -1;
        bytes += s;
    }
    return bytes;
}
double FFDemux::length() const
{
    double length = -1.0;
    for (const FormatContext *fmtCtx : formatContexts)
        length = qMax(length, fmtCtx->length());
    return length;
}
int FFDemux::bitrate() const
{
    int bitrate = 0;
    for (const FormatContext *fmtCtx : formatContexts)
        bitrate += fmtCtx->bitrate();
    return bitrate;
}
QByteArray FFDemux::image(bool forceCopy) const
{
    if (formatContexts.count() == 1)
        return formatContexts.at(0)->image(forceCopy);
    return QByteArray();
}

bool FFDemux::localStream() const
{
    for (const FormatContext *fmtCtx : formatContexts)
        if (!fmtCtx->isLocal)
            return false;
    return true;
}

bool FFDemux::seek(int pos, bool backward)
{
    bool seeked = false;
    for (FormatContext *fmtCtx : formatContexts)
    {
        if (fmtCtx->seek(pos, backward))
            seeked |= true;
        else if (fmtCtx->isStreamed && formatContexts.count() > 1)
        {
            fmtCtx->setStreamOffset(pos);
            seeked |= true;
        }
    }
    return seeked;
}
bool FFDemux::read(Packet &encoded, Decoder::CodeType &codeType)
{   
    double timeBase = 0.0;
    AVPacketRAII avPacketRAII(packet_);

    int result = av_read_frame(formatCtx_, packet_);

    if(0 == result)
    {
        const int ff_idx = packet_->stream_index;
        if (ff_idx >= formatCtx_->nb_streams)
        {
            qDebug() << "Stream index out of range: " << QString::number(ff_idx);
            return false;
        }
        encoded.assign(packet_->buf, packet_->size);
        packet_->buf = nullptr;
        /**
        ffmpeg存在多个时间基准(time_base)，对应不同的阶段(结构体)，
        每个time_base具体的值不一样，ffmpeg提供函数在各个time_base中进行切换。
        搞清楚各个time_base的来源，对于阅读ffmpeg的代码很重要。
        typedef struct AVRational{
            int num; ///< Numerator
            int den; ///< Denominator
        } AVRational;
          */
        if(streamInfo_.audioIndex_ == packet_->stream_index)
        {
            // AAC码流时, time_base.den = 4800为采样率
            // packet_->duration = 1024 为每帧采样点的数量
            timeBase = av_q2d(streamInfo_.audioStream_->time_base);
            //qDebug() << "aduio timeBase: " << timeBase;
            codeType = Decoder::kAudioDecoder;
        }
        else if(streamInfo_.videoIndex_ == packet_->stream_index)
        {
//          有一些帧率frame_rate[每秒出现多少帧]和frame time一帧多少时间（和帧率互为倒数），
//          不能用一个小数比如23.976来精确表示。所以用一个分母和一个分子来表示， 使用125/2997
            timeBase = av_q2d(streamInfo_.videoStream_->time_base);
            //qDebug() << "video timeBase: " << timeBase;
            codeType = Decoder::kVideoDecoder;
        }
        if (packet_->duration > 0)
        {
            // 如果为48Khz采样率的AAC码流，则持续时间为 1024*
            encoded.duration = packet_->duration * timeBase;    // 换算得到每个packet的时长,单位为秒
        }
        //qDebug() << "packet_->duration * timeBase " << encoded.duration;

//      不同的流pts的格式是不同的
//      这里的pts不是以秒为单位的, 是packet->duration的叠加, 比如audio的pts
//      a 16384 17408 18432 18432+1024
//      v 1000 1125 1250 1250+125
//      packet_->pts * timeBase换算后单位也为秒
        encoded.ts.setPts(packet_->pts * timeBase, startTime_);
        return true;
    }
    else
    {
        qDebug() << "FFDemux::read result : " << result;
        // 如果返回false则认为数据读取完了
        isEof_ = true;
        return false;
    }
}
void FFDemux::pause()
{

}
void FFDemux::abort()
{
    QMutexLocker mL(&mutex);
    for (FormatContext *fmtCtx : formatContexts)
        fmtCtx->abort();
    abortFetchTracks = true;
}

bool FFDemux::isEof()
{
    return isEof_;
}

bool FFDemux::open(const QString &entireUrl)
{
    int ret;
    QString prefix, url, param;

    entireUrl_ = entireUrl;
    // 读取文件头，将格式相关信息存放在AVFormatContext结构体中
    ret = avformat_open_input(&formatCtx_, entireUrl_.toUtf8().data(), inputFmt_, &dictOptions_);
    if(ret != 0)
    {
        qDebug() << "avformat_open_input failed, ret = " << ret;
        return false;
    }

    formatCtx_->pb->write_packet = write_packet;
    qDebug() << "avformat_open_input successful";

    ret = avformat_find_stream_info(formatCtx_, nullptr);
    if (ret < 0)
    {
        qDebug() << "avformat_find_stream_info failed, ret = " << ret;
        avformat_close_input(&formatCtx_);
        return false;
    }

    //查找第一个视频流 video stream
    streamInfo_.audioIndex_ = -1;
    streamInfo_.videoIndex_ = -1;
    qDebug() << "formatCtx_->nb_streams = " << formatCtx_->nb_streams;
    for (unsigned int i = 0; i < formatCtx_->nb_streams; ++i)
    {
        if (AVMEDIA_TYPE_AUDIO == formatCtx_->streams[i]->codec->codec_type)
        {
            streamInfo_.audioIndex_ = i;
            streamInfo_.audioStream_ = formatCtx_->streams[i];
            streamInfo_.audioCodeContex_ = formatCtx_->streams[i]->codec; // codec context
            qDebug() << "codec_type = " << streamInfo_.audioCodeContex_->codec_type;
            continue;
        }
        else if (AVMEDIA_TYPE_VIDEO == formatCtx_->streams[i]->codec->codec_type)
        {
            streamInfo_.videoIndex_ = i;
            streamInfo_.videoStream_ = formatCtx_->streams[i];
            streamInfo_.videoCodeContex_ = formatCtx_->streams[i]->codec; // codec context
            continue;
        }
        else
        {
            qDebug() << "formatCtx_->streams[" << i << "]->codec = " << formatCtx_->streams[i]->codec;
        }
    }

    // 3. 根据读取到的流信息查找相应的解码器并打开
    if (-1 == streamInfo_.audioIndex_ && -1 == streamInfo_.videoIndex_)
    {
        qDebug()<<"find no stream";
        avformat_close_input(&formatCtx_);
        return false; // 没有检测到流信息 stream infomation
    }


    if(-1 != streamInfo_.audioIndex_)
    {
        streamInfo_.pAudioCodec_ = avcodec_find_decoder(streamInfo_.audioCodeContex_->codec_id);
        if(streamInfo_.pAudioCodec_)
        {
            ret = avcodec_open2(streamInfo_.audioCodeContex_, streamInfo_.pAudioCodec_, nullptr);
        }
        else
        {
            qDebug()<<"find audio decoder error";
            freeCommSpace();
            return false;
        }
    }

    if(-1 != streamInfo_.videoIndex_)
    {
        streamInfo_.pVideoCodec_ = avcodec_find_decoder(streamInfo_.videoCodeContex_->codec_id);
        if(streamInfo_.pVideoCodec_)
        {
            ret = avcodec_open2(streamInfo_.videoCodeContex_, streamInfo_.pVideoCodec_, nullptr);
        }
        else
        {
            qDebug()<<"find video decoder error";
            freeCommSpace();

            return false;
        }
    }

    if ((startTime_ = formatCtx_->start_time / (double)AV_TIME_BASE) < 0.0)
        startTime_ = 0.0;
    return true;
}

bool FFDemux::init(StreamInfo &streamInfo)
{
    streamInfo_ = streamInfo;
}

void FFDemux::addFormatContext(QString url, const QString &param)
{

}

inline int64_t FFDemux::getDuration()
{
    if(formatCtx_)
    {
        return formatCtx_->duration;
    }
    else
    {
        return 0;
    }
}
