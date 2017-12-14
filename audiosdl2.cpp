#include "audiosdl2.hpp"
#include <QDebug>
#define MAX_AUDIO_FRAME_SIZE  192000
#define SDL_AUDIO_BUFFER_SIZE  1024
void audioCallback(void* userdata, Uint8* stream, int len)
{
    int audio_len = 0;
    int audioVol = 50;
    //MediaState* media_state = (MediaState*)userdata;
    uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE];
    memset(stream, 0, len);
    static int audio_buf_pos=0;
    static int audio_buf_size=0;
    //使用静态变量的原因是audio_buf_size一次数据长度为4608,mp3哈，回调的len又太小，不能让它每执行audio_callback函数就来执行audio_decode_frame解码函数

    while(len>0) //要读len这么长的数据长能退出
    {
        if(audio_buf_pos>=audio_buf_size)//如果完成
        {
            audio_buf_size = 0;// 从缓冲区读取数据
            if (audio_buf_size < 0)
                return;
            audio_buf_pos=0;
        }

        int audio_len= audio_buf_size-audio_buf_pos;
        if(audio_len>len)
            audio_len=len;


        SDL_MixAudio(stream, audio_buff+audio_buf_pos, audio_len, audioVol);
        len-=audio_len;
        audio_buf_pos+=audio_len;
        stream+=audio_len;
    }
}

AudioSdl2::AudioSdl2()
{
    // Set audio settings from codec info
    SDL_AudioSpec wanted_spec, spec;
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
}
AudioSdl2::~AudioSdl2()
{
    SDL_CloseAudio();
}
bool AudioSdl2::write(unsigned char *data, int size)
{
    //dst为混合音频的目的地，src为将要被混合的源音频缓冲区，len为音频缓冲区的长度，volume为范围从0 - 128,应该设置为SDL_MIX_MAXVOLUME完全音频音量。
    SDL_MixAudio(data, data, size, 50);
}
