#include "audiothr.h"
#include "player.hpp"

AudioThr::AudioThr(LPlayer &player, const QStringList &pluginsName) :
     AVThread(player, "audio:", nullptr, pluginsName)
{

}
AudioThr::~AudioThr()
{

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

void AudioThr::run()
{
    setPriority(QThread::HighestPriority);

    while (!br)
    {
        const bool flushAudio = playC.flushAudio;
        Packet packet;
        int bytes_consumed = -1;
        double delay = 0.0, audio_pts = 0.0; //"audio_pts" odporny na zerowanie przy przewijaniu
        Decoder *last_dec = dec;
        playC.aPackets.lock();
        const bool hasAPackets = playC.aPackets.canFetch();
        playC.aPackets.unlock();
        if(hasAPackets)
        {
            // 先检测下输出buffer是否已经满了

            packet = playC.aPackets.fetch();
            // 解码
            Buffer decoded;
            quint8 newChannels = 0;
            quint32 newSampleRate = 0;
            bytes_consumed = dec->decodeAudio(packet, decoded, newChannels, newSampleRate, flushAudio);
            // 把数据写入filter
            //

        }
    }
}

void AudioThr::stop(bool terminate)
{

    for (AudioFilter *filter : filters)
        delete filter;

    AVThread::stop(terminate);
}
