#ifndef DECODER_H
#define DECODER_H

#include <QString>
#include "packetqueue.h"
struct AVCodec;
struct AVFrame;
class StreamInfo;
class Packet;
class VideoFrame;
class Buffer;
class QMPlay2PixelFormats;
class Decoder
{
public:
    enum CodeType{
        kAudioDecoder = 0,
        kVideoDecoder,
        kSubtitleDecoder,
        kUnkownDecoder
                 };
    Decoder();
    virtual QString name() const = 0;
    virtual AVCodec *init(StreamInfo &streamInfo, Decoder::CodeType type) = 0;
    /*
     * hurry_up ==  0 -> no frame skipping, normal quality
     * hurry_up >=  1 -> faster decoding, lower image quality, frame skipping during decode
     * hurry_up == ~0 -> much faster decoding, no frame copying
    */
    virtual int decodeVideo(Packet &encodedPacket , VideoFrame &decoded, QByteArray &newPixFmt, bool flush, unsigned hurry_up);
    virtual int decodeAudio( AVPacketItem *encodedPacket, uint8_t *pcmBuf, AVFrame *wantedFrame, bool flush);
    virtual void setSupportedPixelFormats(const QMPlay2PixelFormats &pixelFormats);
};

#endif // DECODER_H
