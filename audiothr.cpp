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
}

void AudioThr::stop(bool terminate)
{

    for (AudioFilter *filter : filters)
        delete filter;

    AVThread::stop(terminate);
}
