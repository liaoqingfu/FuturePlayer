#include "ffdec.hpp"
#include "functions.hpp"
#include "pixelformat.hpp"

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavutil/pixdesc.h>
}


FFDec::FFDec() :
    pCodecCtx_(nullptr),
    packet(nullptr),
    frame(nullptr),
    codecIsOpen(false)
{

}
FFDec::~FFDec()
{
    av_frame_free(&frame);
    FFCommon::freeAVPacket(packet);
    if (codecIsOpen)
    {
        avcodec_mutex.lock();
        avcodec_close(pCodecCtx_);
        avcodec_mutex.unlock();
    }
    av_free(pCodecCtx_);
}


AVCodec *FFDec::init(StreamInfo &streamInfo, Decoder::CodeType type)
{
    if(Decoder::kAudioDecoder == type)
    {
        codec_ = streamInfo.pAudioCodec_;
        pCodecCtx_ = streamInfo.audioCodeContex_;
    }
    else if(Decoder::kVideoDecoder == type)
    {
        codec_ = streamInfo.pVideoCodec_;
        pCodecCtx_ = streamInfo.videoCodeContex_;
    }
    return codec_;
}
bool FFDec::openCodec(AVCodec *codec)
{
    avcodec_mutex.lock();
    if (avcodec_open2(pCodecCtx_, codec, nullptr))
    {
        avcodec_mutex.unlock();
        return false;
    }
    avcodec_mutex.unlock();
    packet = FFCommon::createAVPacket();
    switch (pCodecCtx_->codec_type)
    {
        case AVMEDIA_TYPE_VIDEO:
        case AVMEDIA_TYPE_AUDIO:
            frame = av_frame_alloc();
            break;
        default:
            break;
    }
    return (codecIsOpen = true);
}

void FFDec::decodeFirstStep(const Packet &encodedPacket, bool flush)
{
    packet->data = (quint8 *)encodedPacket.data();
    packet->size = encodedPacket.size();
    packet->dts = round(encodedPacket.ts.dts() / time_base);
    packet->pts = round(encodedPacket.ts.pts() / time_base);
    if (flush)
        avcodec_flush_buffers(pCodecCtx_);
    if (pCodecCtx_->codec_type == AVMEDIA_TYPE_VIDEO)
        memcpy(&pCodecCtx_->reordered_opaque, &encodedPacket.sampleAspectRatio, 8);
}
void FFDec::decodeLastStep(Packet &encodedPacket, AVFrame *frame)
{
    const int64_t ts = av_frame_get_best_effort_timestamp(frame);
    if (ts != PLAYER_NOPTS_VALUE)
        encodedPacket.ts = ts * time_base;
    if (pCodecCtx_->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        double sampleAspectRatio;
        memcpy(&sampleAspectRatio, &frame->reordered_opaque, 8);
        if (qFuzzyIsNull(sampleAspectRatio) && frame->sample_aspect_ratio.num)
            encodedPacket.sampleAspectRatio = av_q2d(frame->sample_aspect_ratio);
    }
}

int FFDec::decodeAudio(Packet &encodedPacket, Buffer &decoded, quint8 &channels, quint32 &sampleRate, bool flush)
{
    int bytes_consumed = 0, frameFinished = 0;

    decodeFirstStep(encodedPacket, flush);
    if (codec_)
    {
        bytes_consumed = avcodec_decode_audio4(pCodecCtx_, frame, &frameFinished, packet);
        if (frameFinished)
        {
            const int samples_with_channels = frame->nb_samples * pCodecCtx_->channels;
            const int decoded_size = samples_with_channels * sizeof(float);
            decoded.resize(decoded_size);
            float *decoded_data = (float *)decoded.data();
            switch (pCodecCtx_->sample_fmt)
            {
                case AV_SAMPLE_FMT_U8:
                {
                    uint8_t *data = (uint8_t *)*frame->data;
                    for (int i = 0; i < samples_with_channels; i++)
                        decoded_data[i] = (data[i] - 0x7F) / 128.0f;
                } break;
                case AV_SAMPLE_FMT_S16:
                {
                    int16_t *data = (int16_t *)*frame->data;
                    for (int i = 0; i < samples_with_channels; i++)
                        decoded_data[i] = data[i] / 32768.0f;
                } break;
                case AV_SAMPLE_FMT_S32:
                {
                    int32_t *data = (int32_t *)*frame->data;
                    for (int i = 0; i < samples_with_channels; i++)
                        decoded_data[i] = data[i] / 2147483648.0f;
                } break;
                case AV_SAMPLE_FMT_FLT:
                    memcpy(decoded_data, *frame->data, decoded_size);
                    break;
                case AV_SAMPLE_FMT_DBL:
                {
                    double *data = (double *)*frame->data;
                    for (int i = 0; i < samples_with_channels; i++)
                        decoded_data[i] = data[i];
                } break;

                /* Thanks Wang Bin for this patch */
                case AV_SAMPLE_FMT_U8P:
                {
                    uint8_t **data = (uint8_t **)frame->extended_data;
                    for (int i = 0; i < frame->nb_samples; ++i)
                        for (int ch = 0; ch < pCodecCtx_->channels; ++ch)
                            *decoded_data++ = (data[ch][i] - 0x7F) / 128.0f;
                } break;
                case AV_SAMPLE_FMT_S16P:
                {
                    int16_t **data = (int16_t **)frame->extended_data;
                    for (int i = 0; i < frame->nb_samples; ++i)
                        for (int ch = 0; ch < pCodecCtx_->channels; ++ch)
                            *decoded_data++ = data[ch][i] / 32768.0f;
                } break;
                case AV_SAMPLE_FMT_S32P:
                {
                    int32_t **data = (int32_t **)frame->extended_data;
                    for (int i = 0; i < frame->nb_samples; ++i)
                        for (int ch = 0; ch < pCodecCtx_->channels; ++ch)
                            *decoded_data++ = data[ch][i] / 2147483648.0f;
                } break;
                case AV_SAMPLE_FMT_FLTP:
                {
                    float **data = (float **)frame->extended_data;
                    for (int i = 0; i < frame->nb_samples; ++i)
                        for (int ch = 0; ch < pCodecCtx_->channels; ++ch)
                            *decoded_data++ = data[ch][i];
                } break;
                case AV_SAMPLE_FMT_DBLP:
                {
                    double **data = (double **)frame->extended_data;
                    for (int i = 0; i < frame->nb_samples; ++i)
                        for (int ch = 0; ch < pCodecCtx_->channels; ++ch)
                            *decoded_data++ = data[ch][i];
                } break;
                /**/

                default:
                    decoded.clear();
                    break;
            }
            channels = pCodecCtx_->channels;
            sampleRate = pCodecCtx_->sample_rate;
        }
    }

    if (frameFinished)
        decodeLastStep(encodedPacket, frame);
    else
        encodedPacket.ts.setInvalid();

    if (bytes_consumed < 0)
        bytes_consumed = 0;
    return bytes_consumed;
}
int FFDec::decodeVideo(Packet &encodedPacket, VideoFrame &decoded, QByteArray &newPixFmt, bool flush, unsigned hurry_up)
{
    int bytes_consumed = 0, frameFinished = 0;

    decodeFirstStep(encodedPacket, flush);

    if  (codec_)
    {
        if (respectHurryUP && hurry_up)
        {
            if (skipFrames && !forceSkipFrames && hurry_up > 1)
                pCodecCtx_->skip_frame = AVDISCARD_NONREF;
            pCodecCtx_->skip_loop_filter = AVDISCARD_ALL;
            if (hurry_up > 1)
                pCodecCtx_->skip_idct = AVDISCARD_NONREF;
            pCodecCtx_->flags2 |= CODEC_FLAG2_FAST;
        }
        else
        {
            if (!forceSkipFrames)
                pCodecCtx_->skip_frame = AVDISCARD_DEFAULT;
            pCodecCtx_->skip_loop_filter = pCodecCtx_->skip_idct = AVDISCARD_DEFAULT;
            pCodecCtx_->flags2 &= ~CODEC_FLAG2_FAST;
        }

        bytes_consumed = avcodec_decode_video2(pCodecCtx_, frame, &frameFinished, packet);

        if (forceSkipFrames) //Nie możemy pomijać na pierwszej klatce, ponieważ wtedy może nie być odczytany przeplot
            pCodecCtx_->skip_frame = AVDISCARD_NONREF;

        if (frameFinished && ~hurry_up)
        {
            bool newFormat = false;
            if (pCodecCtx_->pix_fmt != lastPixFmt)
            {
                newPixFmt = av_get_pix_fmt_name(pCodecCtx_->pix_fmt);
                lastPixFmt = pCodecCtx_->pix_fmt;
                setPixelFormat();
                newFormat = true;
            }
            if (desiredPixFmt != AV_PIX_FMT_NONE)
            {
                const VideoFrameSize frameSize(frame->width, frame->height, chromaShiftW, chromaShiftH);
                if (dontConvert && frame->buf[0] && frame->buf[1] && frame->buf[2])
                    decoded = VideoFrame(frameSize, frame->buf, frame->linesize, frame->interlaced_frame, frame->top_field_first);
                else
                {
                    const int aligned8W = Functions::aligned(frame->width, 8);
                    const int linesize[] = {
                        aligned8W,
                        aligned8W >> chromaShiftW,
                        aligned8W >> chromaShiftW
                    };
                    decoded = VideoFrame(frameSize, linesize, frame->interlaced_frame, frame->top_field_first);
                    if (frame->width != lastFrameW || frame->height != lastFrameH || newFormat)
                    {
                        sws_ctx = sws_getCachedContext(sws_ctx, frame->width, frame->height, pCodecCtx_->pix_fmt, frame->width, frame->height, (AVPixelFormat)desiredPixFmt, SWS_BILINEAR, nullptr, nullptr, nullptr);
                        lastFrameW = frame->width;
                        lastFrameH = frame->height;
                    }
                    quint8 *decodedData[] = {
                        decoded.buffer[0].data(),
                        decoded.buffer[1].data(),
                        decoded.buffer[2].data()
                    };
                    sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, decodedData, decoded.linesize);
                }
            }
        }
    }

    if (frameFinished)
        decodeLastStep(encodedPacket, frame);
    else
        encodedPacket.ts.setInvalid();

    return bytes_consumed < 0 ? -1 : bytes_consumed;
}

void FFDec::setPixelFormat()
{
    const AVPixFmtDescriptor *pixDesc = av_pix_fmt_desc_get(pCodecCtx_->pix_fmt);
    if (!pixDesc) //Invalid pixel format
        return;
    dontConvert = supportedPixelFormats.contains((QMPlay2PixelFormat)pCodecCtx_->pix_fmt);
    if (dontConvert)
    {
        chromaShiftW = pixDesc->log2_chroma_w;
        chromaShiftH = pixDesc->log2_chroma_h;
        desiredPixFmt = pCodecCtx_->pix_fmt;
    }
    else for (int i = 0; i < supportedPixelFormats.count(); ++i)
    {
        const AVPixFmtDescriptor *supportedPixDesc = av_pix_fmt_desc_get((AVPixelFormat)supportedPixelFormats.at(i));
        if (i == 0 || (supportedPixDesc->log2_chroma_w == pixDesc->log2_chroma_w && supportedPixDesc->log2_chroma_h == pixDesc->log2_chroma_h))
        {
            //Use first format as default (mostly QMPlay2PixelFormat::YUV420P) and look at next formats,
            //otherwise break the loop if found proper format.
            chromaShiftW = supportedPixDesc->log2_chroma_w;
            chromaShiftH = supportedPixDesc->log2_chroma_h;
            desiredPixFmt = (int)supportedPixelFormats.at(i);
            if (i != 0)
                break;
        }
    }
}

void FFDec::setSupportedPixelFormats(const QMPlay2PixelFormats &pixelFormats)
{
    supportedPixelFormats = pixelFormats;
    setPixelFormat();
}
