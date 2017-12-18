#include "audiothr.h"
#include "player.hpp"
#include "audiosdl2.hpp"

AudioThr::AudioThr(LPlayer &player, const QStringList &pluginsName) :
     AVThread(player, "audio:", nullptr, pluginsName)
{
    pAudioSdl2_ = new AudioSdl2();
}
AudioThr::~AudioThr()
{
    if(pAudioSdl2_)
        delete pAudioSdl2_;
}

#ifdef Q_OS_WIN
void AudioThr::timerEvent(QTimerEvent *)
{
    if (br || !isRunning())
        return;
    if (canUpdatePos)
    {
        //if (!playC.vThr && playC.canUpdatePos)
        //	emit playC.updatePos(round(playC.pos));
        canUpdatePos = false;
    }
    if (canUpdateBitrate)
    {
        //emit playC.updateBitrateAndFPS(round((tmp_br << 3) / tmp_time), -1);
        canUpdateBitrate = false;
        tmp_time = tmp_br = 0;
    }
}
#endif

const int MAX_AUDIO_FRAME_SIZE = 192000;
void AudioThr::run()
{
    setPriority(QThread::HighestPriority);

    uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE];
    while (!br)
    {
        const bool flushAudio = playC.flushAudio;
        AVPacketItem *packet;
        int bytes_consumed = -1;
        double delay = 0.0, audio_pts = 0.0; //"audio_pts" odporny na zerowanie przy przewijaniu
        Decoder *last_dec = dec;

        bool hasAPackets = false;
        if(playC.aPackets.size() > 0)
             hasAPackets = true;


        if(hasAPackets)
        {
            // 先检测下输出buffer是否已经满了

            packet = playC.aPackets.take();
            // 解码
            Buffer decoded;
            quint8 newChannels = 0;
            quint32 newSampleRate = 0;
            bytes_consumed = dec->decodeAudio(packet, audio_buff, pAudioSdl2_->wanted_frame, flushAudio);
            // 把数据写入filter
           if(bytes_consumed > 0)
           {
                if(!pAudioSdl2_->write(audio_buff, bytes_consumed))     // 内存满则休眠
                {
                    msleep(100);
                }
           }
        }
    }
}

void AudioThr::stop(bool terminate)
{

    for (AudioFilter *filter : filters)
        delete filter;

    AVThread::stop(terminate);
}
