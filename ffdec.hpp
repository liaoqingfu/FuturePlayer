#ifndef FFDEC_HPP
#define FFDEC_HPP
#include <QMutex>
#include <buffer.hpp>
#include "ffcommon.h"
#include "packet.h"
#include "decoder.h"
#include "streaminfo.hpp"
#include "videoframe.hpp"
#include "pixelformat.hpp"
class QMutex;
struct AVCodecContext;
struct AVPacket;
struct AVCodec;
struct AVFrame;

class FFDec : public Decoder
{
public:
    FFDec();
    virtual ~FFDec();

    /**/

    AVCodec *init(StreamInfo &streamInfo, Decoder::CodeType type);
    bool openCodec(AVCodec *codec);

    void decodeFirstStep(const Packet &encodedPacket, bool flush);
    void decodeLastStep(Packet &encodedPacket, AVFrame *frame);
    int  decodeAudio(Packet &encodedPacket, Buffer &decoded, quint8 &channels, quint32 &sampleRate, bool flush) override;
    int  decodeVideo(Packet &encodedPacket, VideoFrame &decoded, QByteArray &newPixFmt, bool flush, unsigned hurry_up) override;
    void setPixelFormat();
    void setSupportedPixelFormats(const QMPlay2PixelFormats &pixelFormats);
    virtual QString name() const
    {
        return QString("FFDec");
    }
    AVCodecContext *pCodecCtx_;
    AVPacket *packet;
    AVFrame *frame;
    double time_base;
    bool codecIsOpen;

    QMutex avcodec_mutex;
    AVCodec *codec_;
    int threads, lowres;
    bool respectHurryUP, skipFrames, forceSkipFrames, thread_type_slice;
    int lastFrameW, lastFrameH, lastPixFmt;
    SwsContext *sws_ctx;

    QMPlay2PixelFormats supportedPixelFormats;
    quint8 chromaShiftW, chromaShiftH;
    int desiredPixFmt;
    bool dontConvert;
};
#endif // FFDEC_HPP
