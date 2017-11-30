#include <QCoreApplication>
#include <QDebug>
#include "formatcontext.h"
#include "ffcommon.h"

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

static void matroska_fix_ass_packet(AVRational stream_timebase, AVPacket *pkt)
{
    AVBufferRef *line;
    char *layer, *ptr = (char *)pkt->data, *end = ptr + pkt->size;
    for (; *ptr != ',' && ptr < end - 1; ptr++);
    if (*ptr == ',')
        ptr++;
    layer = ptr;
    for (; *ptr != ',' && ptr < end - 1; ptr++);
    if (*ptr == ',')
    {
        int64_t end_pts = pkt->pts + pkt->duration;
        int sc = pkt->pts * stream_timebase.num * 100 / stream_timebase.den;
        int ec = end_pts  * stream_timebase.num * 100 / stream_timebase.den;
        int sh, sm, ss, eh, em, es, len;
        sh     = sc / 360000;
        sc    -= 360000 * sh;
        sm     = sc / 6000;
        sc    -= 6000 * sm;
        ss     = sc / 100;
        sc    -= 100 * ss;
        eh     = ec / 360000;
        ec    -= 360000 * eh;
        em     = ec / 6000;
        ec    -= 6000 * em;
        es     = ec / 100;
        ec    -= 100 * es;
        *ptr++ = '\0';
        len    = 50 + end - ptr + FF_INPUT_BUFFER_PADDING_SIZE;
        if (!(line = av_buffer_alloc(len)))
            return;
        snprintf((char *)line->data, len, "Dialogue: %s,%d:%02d:%02d.%02d,%d:%02d:%02d.%02d,%s\r\n", layer, sh, sm, ss, sc, eh, em, es, ec, ptr);
        av_buffer_unref(&pkt->buf);
        pkt->buf  = line;
        pkt->data = line->data;
        pkt->size = strlen((char *)line->data);
    }
}

#if LIBAVFORMAT_VERSION_INT >= 0x392900
    static inline AVCodecParameters *codecParams(AVStream *stream)
    {
        return stream->codecpar;
    }
    static inline const char *getPixelFormat(AVStream *stream)
    {
        return av_get_pix_fmt_name((AVPixelFormat)stream->codecpar->format);
    }
    static inline const char *getSampleFormat(AVStream *stream)
    {
        return av_get_sample_fmt_name((AVSampleFormat)stream->codecpar->format);
    }
#else
    static inline AVCodecContext *codecParams(AVStream *stream)
    {
        return stream->codec;
    }
    static inline const char *getPixelFormat(AVStream *stream)
    {
        return av_get_pix_fmt_name(stream->codec->pix_fmt);
    }
    static inline const char *getSampleFormat(AVStream *stream)
    {
        return av_get_sample_fmt_name(stream->codec->sample_fmt);
    }
#endif

FormatContext::FormatContext(QMutex &avcodec_mutex, bool reconnectStreamed):
    isError(false),
    currPos(0.0),
    reconnectStreamed(reconnectStreamed),
    avcodec_mutex(avcodec_mutex)
{

}
FormatContext::~FormatContext()
{
    if (formatCtx)
    {
        for (AVStream *stream : streams)
        {
            if (codecParams(stream))
            {
                //Data is allocated in QByteArray, so FFmpeg mustn't free it!
                codecParams(stream)->extradata = nullptr;
                codecParams(stream)->extradata_size = 0;
            }
        }
        avformat_close_input(&formatCtx);
        FFCommon::freeAVPacket(packet);
    }
}


QString FormatContext::name() const
{
    return formatCtx->iformat->name;
}

QString FormatContext::title() const
{
    return QString();
}
qint64 FormatContext::size() const
{
    if (!isStreamed && !stillImage && formatCtx->pb)
        return avio_size(formatCtx->pb);
    return -1;
}

double FormatContext::length() const
{
    if (!isStreamed && !stillImage && formatCtx->duration != AV_NOPTS_VALUE)
        return formatCtx->duration / (double)AV_TIME_BASE;
    return -1.0;
}

int FormatContext::bitrate() const
{
    return formatCtx->bit_rate / 1000;
}

bool FormatContext::seek(double pos, bool backward)
{
    bool isOk = false;

    if (!isStreamed)
    {
        const double len = length();
        if (pos < 0.0)
            pos = 0.0;
        else if (len > 0.0 && pos > len)
            pos = len;

        const double posToSeek = pos + startTime;
        const qint64 timestamp =  posToSeek ;

        isOk = av_seek_frame(formatCtx, -1, timestamp, backward ? AVSEEK_FLAG_BACKWARD : 0) >= 0;
        if (!isOk)
        {
            const int ret = av_read_frame(formatCtx, packet);
            if (ret == AVERROR_EOF || ret == 0)
            {
                if (len <= 0.0 || pos < len)
                    isOk = av_seek_frame(formatCtx, -1, timestamp, !backward ? AVSEEK_FLAG_BACKWARD : 0) >= 0; //Negate "backward" and try again
                else if (ret == AVERROR_EOF)
                    isOk = true; //Allow seek to the end of the file, clear buffers and finish the playback
                if (isOk)
                    av_packet_unref(packet);
            }
            if (!isOk) //If seek failed - allow to use the packet
            {
                errFromSeek = ret;
                maybeHasFrame = true;
            }
        }

        if (isOk)
        {
            for (int i = 0; i < streamsTS.count(); ++i)
                streamsTS[i] = pos;
            currPos = pos;
            nextDts.fill(pos);
            isError = false;
        }
    }
    return isOk;
}
bool FormatContext::read(Packet &encoded, int &idx)
{
    if (isPaused)
    {
        isPaused = false;
        av_read_play(formatCtx);
    }

    AVPacketRAII avPacketRAII(packet);

    int ret;

    ret = av_read_frame(formatCtx, packet);


    if (ret == AVERROR_INVALIDDATA)
    {
        if (invalErrCount < 1000)
        {
            ++invalErrCount;
            return true;
        }
        isError = true;
        return false;
    }
    else
        invalErrCount = 0;
    if (ret == AVERROR(EAGAIN))
        return true;
    else if (ret)
    {
        isError = true;
        return false;
    }

    const int ff_idx = packet->stream_index;
    if (ff_idx >= streams.count())
    {
        qDebug() << "Stream index out of range: " << QString::number(ff_idx);
        return true;
    }

    if (streams.at(ff_idx)->event_flags & AVSTREAM_EVENT_FLAG_METADATA_UPDATED)
    {
        streams.at(ff_idx)->event_flags = 0;
        isMetadataChanged = true;
    }
    if (fixMkvAss && codecParams(streams.at(ff_idx))->codec_id == AV_CODEC_ID_ASS)
        matroska_fix_ass_packet(streams.at(ff_idx)->time_base, packet);

    if (!packet->buf) //Buffer isn't reference-counted, so copy the data
        encoded.assign(packet->data, packet->size, packet->size + FF_INPUT_BUFFER_PADDING_SIZE);
    else
    {
        encoded.assign(packet->buf, packet->size);
        packet->buf = nullptr;
    }

    const double time_base = av_q2d(streams.at(ff_idx)->time_base);

    {
        encoded.ts.setInvalid();
        if (packet->dts != AV_NOPTS_VALUE)
            encoded.ts.setDts(packet->dts * time_base, startTime);
        if (packet->pts != AV_NOPTS_VALUE)
            encoded.ts.setPts(packet->pts * time_base, startTime);
    }


    if (packet->duration > 0)
        encoded.duration = packet->duration * time_base;
    else if (!encoded.ts || (encoded.duration = encoded.ts - streamsTS.at(ff_idx)) < 0.0 /* Calculate packet duration if doesn't exists */)
        encoded.duration = 0.0;
    streamsTS[ff_idx] = encoded.ts;

    if (isStreamed)
    {

        encoded.ts = lastTime;
        lastTime += encoded.duration;

    }

    currPos = encoded.ts;

    encoded.hasKeyFrame = packet->flags & AV_PKT_FLAG_KEY;
    if (streams.at(ff_idx)->sample_aspect_ratio.num)
        encoded.sampleAspectRatio = av_q2d(streams.at(ff_idx)->sample_aspect_ratio);

    // Generate DTS for key frames if DTS doesn't exist (workaround for some M3U8 seekable streams)
    if (encoded.hasKeyFrame && !encoded.ts.hasDts())
        encoded.ts.setDts(nextDts.at(ff_idx));
    nextDts[ff_idx] = encoded.ts + encoded.duration;

    idx = index_map.at(ff_idx);

    return true;
}
void FormatContext::pause()
{
    av_read_pause(formatCtx);
    isPaused = true;
}

void FormatContext::abort()
{

}
static int interruptCB(bool &aborted)
{
    QCoreApplication::processEvents(); //Let the demuxer thread run the timer
    return aborted;
}

static int write_packet(void *opaque, uint8_t *buf, int buf_size)
{
    qDebug()<<buf_size;

    return buf_size;
}

static QByteArray getTag(AVDictionary *metadata, const char *key, const bool deduplicate = true)
{
    AVDictionaryEntry *avTag = av_dict_get(metadata, key, nullptr, AV_DICT_IGNORE_SUFFIX);
    if (avTag && avTag->value)
    {
        const QByteArray tag = QByteArray(avTag->value);
        if (deduplicate)
        {
            // Workaround for duplicated tags separated by ';'.
            // Check only when both tag has the same length and use only letters and numbers for
            // comparision (sometimes it differs in apostrophe or different/incorrect encoding).
            // Return the second tag (mostly better).
            const QList<QByteArray> tags = tag.split(';');
            if (tags.count() == 2)
            {
                const QByteArray first  = tags[0].trimmed();
                const QByteArray second = tags[1].trimmed();
                if (first.length() == second.length())
                {
                    bool ok = true;
                    for (int i = 0; i < second.length(); ++i)
                    {
                        const char c1 = first[i];
                        const char c2 = second[i];
                        if
                        (
                            (c2 >= '0' && c2 <= '9' && c1 != c2) ||
                            (
                                ((c2 >= 'a' && c2 <= 'z') || (c2 >= 'A' && c2 <= 'Z')) &&
                                ((c1 | 0x20) != (c2 | 0x20))
                            )
                        )
                        {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                        return second;
                }
            }
        }
        return tag.trimmed();
    }
    return QByteArray();
}

static void fixFontsAttachment(AVStream *stream)
{
    if (codecParams(stream)->codec_type == AVMEDIA_TYPE_ATTACHMENT && codecParams(stream)->codec_id == AV_CODEC_ID_NONE)
    {
        // If codec for fonts is unknown - check the attachment file name extension
        const QString attachmentFileName = getTag(stream->metadata, "filename", false);
        if (attachmentFileName.endsWith(".otf", Qt::CaseInsensitive))
            codecParams(stream)->codec_id = AV_CODEC_ID_OTF;
        else if (attachmentFileName.endsWith(".ttf", Qt::CaseInsensitive))
            codecParams(stream)->codec_id = AV_CODEC_ID_TTF;
    }
}

bool FormatContext::open(const QString &_url, const QString &param)
{
    static const QStringList disabledDemuxers {
        "ass",
        "tty" //txt files
    };

    const QByteArray scheme = _url.toUtf8();
    if (scheme.isEmpty() || scheme == "sftp")
        return false;



    qint64 oggOffset = -1, oggSize = -1;
    int oggTrack = -1;
    QString url;

    if (param.startsWith("OGG:")) //For chained OGG files
    {
        const QStringList splitted = param.split(':');
        if (splitted.count() != 4)
            return false;
        oggTrack = splitted[1].toInt();
        oggOffset = splitted[2].toLongLong();
        oggSize = splitted[3].toLongLong();
        if (oggTrack <= 0 || oggOffset < 0 || (oggSize != -1 && oggSize <= 0))
            return false;
    }

    AVInputFormat *inputFmt = nullptr;
    if (scheme == "file")
        isLocal = true;
    else
    {
        inputFmt = av_find_input_format(scheme);
        if (inputFmt)
            url = _url.right(_url.length() - scheme.length() - 3);
        isLocal = false;
    }

    AVDictionary *options = nullptr;
    if (!inputFmt)
    {
        url = _url;
#if LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(57, 25, 100)
        if (!isLocal && reconnectStreamed)
            av_dict_set(&options, "reconnect_streamed", "1", 0);
#endif
    }

    formatCtx = avformat_alloc_context();
    formatCtx->interrupt_callback.callback = (int(*)(void *))interruptCB;
   // formatCtx->interrupt_callback.opaque = &abortCtx->isAborted;



    // Useful, e.g. CUVID decoder needs valid PTS
    formatCtx->flags |= AVFMT_FLAG_GENPTS;

    if (avformat_open_input(&formatCtx, url.toUtf8().data(), nullptr, nullptr) != 0)
    {
        qDebug()<<"open error";
        return false; // 打开失败
    }

    if (!formatCtx || disabledDemuxers.contains(name()))
        return false;
    formatCtx->pb->write_packet = write_packet;

    avcodec_mutex.lock();
    if (avformat_find_stream_info(formatCtx, nullptr) < 0)
    {
        avcodec_mutex.unlock();
        return false;
    }
    avcodec_mutex.unlock();

    isStreamed = !isLocal && formatCtx->duration <= 0; //QMPLAY2_NOPTS_VALUE is negative




    if ((startTime = formatCtx->start_time / (double)AV_TIME_BASE) < 0.0)
        startTime = 0.0;

    index_map.resize(formatCtx->nb_streams);
    streamsTS.resize(formatCtx->nb_streams);
    streamsOffset.resize(formatCtx->nb_streams);
    nextDts.resize(formatCtx->nb_streams);
    for (unsigned i = 0; i < formatCtx->nb_streams; ++i)
    {
        fixFontsAttachment(formatCtx->streams[i]);

        if (!fixMkvAss && codecParams(formatCtx->streams[i])->codec_id == AV_CODEC_ID_ASS && !strncasecmp(formatCtx->iformat->name, "matroska", 8))
            fixMkvAss = true;
        formatCtx->streams[i]->event_flags = 0;
        streams += formatCtx->streams[i];

        streamsTS[i] = 0.0;
    }



    formatCtx->event_flags = 0;

    packet = FFCommon::createAVPacket();

    return true;
}
void FormatContext::setStreamOffset(double offset)
{


}

QByteArray FormatContext::image(bool forceCopy) const
{

}
