#include "audiosdl2.hpp"
#include "ringbuffer.hpp"

#include <QDebug>
#define MAX_AUDIO_FRAME_SIZE  192000
#define SDL_AUDIO_BUFFER_SIZE  1024
void audioCallback(void* userdata, Uint8* stream, int len)
{
    AudioSdl2 *pAudioSdl2 = (AudioSdl2 *)userdata;
    uint8_t *audio_buff;
    memset(stream, 0, len);
    static int audio_buf_pos = 0;
    static int audio_buf_size = 0;
    //使用静态变量的原因是audio_buf_size一次数据长度为4608,mp3哈，回调的len又太小，不能让它每执行audio_callback函数就来执行audio_decode_frame解码函数

    qDebug() << "audioCallback, len = " << len;
    while(len > 0) //要读len这么长的数据长能退出
    {
        if(audio_buf_pos >=audio_buf_size)//如果完成
        {
            audio_buff = ( uint8_t *)pAudioSdl2->pcmRingBuffer_->RingBufferSrSwGet(&audio_buf_size);
            audio_buf_pos = 0;
            if (audio_buf_size <= 0)
            {
                qDebug() << "no pcm buffer" << endl;
                return;
            }
        }

        int audio_len= audio_buf_size - audio_buf_pos;
        if(audio_len>len)
            audio_len = len;


        SDL_MixAudio(stream, audio_buff+audio_buf_pos, audio_len, 100);
        len -= audio_len;
        audio_buf_pos += audio_len;
        stream += audio_len;
    }
}

AudioSdl2::AudioSdl2()
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        qDebug() << "SDL_Init failed:" << SDL_GetError() << endl;
    }
    // Set audio settings from codec info

    wanted_spec.freq = 48000;//media_state_.acct->sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = 2;
    wanted_spec.silence = 0;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    wanted_spec.callback = audioCallback;
    wanted_spec.userdata = this;
    // CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
    {
        qDebug() << "Open audio failed:" << SDL_GetError() << endl;
    }
    else
    {
        qDebug() << "SDL_OpenAudio init" << endl;
    }
    pcmRingBuffer_ = new RingBuffer();
   // SDL_PauseAudio(0);        // 需要调用该函数才能触发调用audioCallback
}
AudioSdl2::~AudioSdl2()
{
    SDL_CloseAudio();
    delete pcmRingBuffer_;
}
bool AudioSdl2::write(unsigned char *data, int size)
{
    //dst为混合音频的目的地，src为将要被混合的源音频缓冲区，len为音频缓冲区的长度，volume为范围从0 - 128,应该设置为SDL_MIX_MAXVOLUME完全音频音量。
    if(pcmRingBuffer_->RingBufferSrSwPut(data, size) != 0)
    {
        //qDebug() << "buffer size = " << pcmRingBuffer_->RingBufferSize();
        return false;
    }
    else
    {
        return true;
    }
}
