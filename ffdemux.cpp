#include "ffdemux.h"
#include "formatcontext.h"
#include "demuxerfactory.hpp"

#include <qDebug>

int write_packet(void *opaque, uint8_t *buf, int buf_size)
{
    qDebug()<< "write_packet = " << buf_size;

    return buf_size;
}

inline void FFDemux::freeCommSpace()
{
    avformat_close_input(&formatCtx_);
}

FFDemux::FFDemux()
{
    qDebug() << "FFDemux init";
    inputFmt_ = nullptr;
    dictOptions_ = nullptr;
    formatCtx_ = nullptr;
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
bool FFDemux::read(Packet &encoded, int &idx)
{
    int fmtCtxIdx = -1;
    int numErrors = 0;

    double ts;
    for (int i = 0; i < formatContexts.count(); ++i)
    {
        FormatContext *fmtCtx = formatContexts.at(i);
        if (fmtCtx->isError)
        {
            ++numErrors;
            continue;
        }
        if (fmtCtxIdx < 0 || fmtCtx->currPos < ts)
        {
            ts = fmtCtx->currPos;
            fmtCtxIdx = i;
        }
    }

    if (fmtCtxIdx < 0) //Every format context has an error
        return false;

    if (formatContexts.at(fmtCtxIdx)->read(encoded, idx))
    {
        for (int i = 0; i < fmtCtxIdx; ++i)
            idx += formatContexts.at(i)->streamsInfo.count();
        return true;
    }

    return numErrors < formatContexts.count() - 1; //Not Every format context has an error
}
void FFDemux::pause()
{
    for (FormatContext *fmtCtx : formatContexts)
        fmtCtx->pause();
}
void FFDemux::abort()
{
    QMutexLocker mL(&mutex);
    for (FormatContext *fmtCtx : formatContexts)
        fmtCtx->abort();
    abortFetchTracks = true;
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
    return true;
}

bool FFDemux::init(StreamInfo &streamInfo)
{
    streamInfo_ = streamInfo;
}

void FFDemux::addFormatContext(QString url, const QString &param)
{

}
