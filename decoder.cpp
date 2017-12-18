#include "decoder.h"
#include "buffer.hpp"
#include "packet.h"
Decoder::Decoder()
{

}
int Decoder::decodeVideo(Packet &encodedPacket, VideoFrame &decoded, QByteArray &newPixFmt, bool flush, unsigned hurry_up)
{
    Q_UNUSED(encodedPacket)
    Q_UNUSED(decoded)
    Q_UNUSED(newPixFmt)
    Q_UNUSED(flush)
    Q_UNUSED(hurry_up)
    return 0;
}
int Decoder::decodeAudio(AVPacketItem *encodedPacket, uint8_t *pcmBuf, AVFrame *wantedFrame, bool flush)
{
    Q_UNUSED(encodedPacket)
    Q_UNUSED(pcmBuf)
    Q_UNUSED(wantedFrame)
     Q_UNUSED(flush)
    return -1;
}

void Decoder::setSupportedPixelFormats(const QMPlay2PixelFormats &pixelFormats)
{
    Q_UNUSED(pixelFormats)
}
