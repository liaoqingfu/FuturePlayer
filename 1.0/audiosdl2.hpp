#ifndef AUDIOSDL2_HPP
#define AUDIOSDL2_HPP

class RingBuffer;
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "SDL.h"
#include "SDL_thread.h"
}

/**
 * @brief The AudioSdl2 class
 * 提供音频输出
 */

class AudioSdl2
{
public:
    AudioSdl2();
    ~AudioSdl2();
    bool setFormat(int sampleRate, int bitDepth);
    bool write(unsigned char *data, int size);
    int getSize();      // 剩余的缓存
    RingBuffer *pcmRingBuffer_;
    SDL_AudioSpec wanted_spec, spec;
    AVFrame *wanted_frame;//音频目标帧
};

#endif // AUDIOSDL2_HPP
