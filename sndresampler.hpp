#ifndef SNDRESAMPLER_HPP
#define SNDRESAMPLER_HPP

#include <stddef.h>
class QByteArray;

class SndResampler
{
public:
    SndResampler() = default;
    inline ~SndResampler()
    {
        destroy();
    }

    const char *name() const;

    inline bool isOpen() const
    {
        return snd_convert_ctx != nullptr;
    }

    bool create(int _src_samplerate, int _src_channels, int _dst_samplerate, int _dst_channels);
    void convert(const QByteArray &src, QByteArray &dst);
    void destroy();
private:
#ifdef QMPLAY2_AVRESAMPLE
    struct AVAudioResampleContext *snd_convert_ctx = nullptr;
#else
    struct SwrContext *snd_convert_ctx = nullptr;
#endif
    int src_samplerate = 0, src_channels = 0, dst_samplerate = 0, dst_channels = 0;
};

#endif // SNDRESAMPLER_HPP
