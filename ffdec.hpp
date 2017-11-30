#ifndef FFDEC_HPP
#define FFDEC_HPP
#include <QMutex>
#include <buffer.hpp>
#include "ffcommon.h"
#include "packet.h"
#include "decoder.h"
#include "streaminfo.hpp"
class QMutex;
struct AVCodecContext;
struct AVPacket;
struct AVCodec;
struct AVFrame;

class FFDec : public Decoder
{
protected:
    FFDec(QMutex &);
    virtual ~FFDec();

    /**/

    AVCodec *init(StreamInfo &streamInfo);
    bool openCodec(AVCodec *codec);

    void decodeFirstStep(const Packet &encodedPacket, bool flush);
    void decodeLastStep(Packet &encodedPacket, AVFrame *frame);

    AVCodecContext *codec_ctx;
    AVPacket *packet;
    AVFrame *frame;
    double time_base;
    bool codecIsOpen;

    QMutex &avcodec_mutex;
};
#endif // FFDEC_HPP
