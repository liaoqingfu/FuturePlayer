#ifndef AUDIOSDL2_HPP
#define AUDIOSDL2_HPP


#include "SDL2/include/SDL_audio.h"
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

};

#endif // AUDIOSDL2_HPP
