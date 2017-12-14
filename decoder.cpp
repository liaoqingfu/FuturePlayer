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
int Decoder::decodeAudio(Packet &encodedPacket, Buffer &decoded, quint8 &channels, quint32 &sampleRate, bool flush)
{
    Q_UNUSED(channels)
    Q_UNUSED(sampleRate)
    Q_UNUSED(flush)
    return (decoded = encodedPacket).size();
}

void Decoder::setSupportedPixelFormats(const QMPlay2PixelFormats &pixelFormats)
{
    Q_UNUSED(pixelFormats)
}
