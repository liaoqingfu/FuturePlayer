#include "player.h"

LPlayer::LPlayer()
{

}

void LPlayer::play(const QString &_url)
{
}

void LPlayer::stop(bool _quitApp)
{

}

void LPlayer::restart()
{

}

void LPlayer::chPos(double newPos, bool updateGUI)
{

}

void LPlayer::togglePause()
{

}

void LPlayer::seek(double pos, bool allowAccurate)
{

}

void LPlayer::chStream(const QString &s)
{

}

void LPlayer::setSpeed(double spd)
{

}

bool LPlayer::isPlaying() const
{
    return demuxThr && demuxThr->isRunning();
}

void LPlayer::loadSubsFile(const QString &fileName)
{

}

void LPlayer::stopVThr()
{
    if (vThr)
    {
        stopVDec();
        if (vThr)
        {
            vThr->stop();
            vThr = nullptr;
        }
    }
}
void LPlayer::stopAThr()
{
    doSilenceOnStart = false;
    if (aThr)
    {
        stopADec();
        if (aThr)
        {
            aThr->stop();
            aThr = nullptr;
        }
    }
}

inline void LPlayer::stopAVThr()
{
    stopVThr();
    stopAThr();
}

void LPlayer::stopVDec()
{
    if (vThr && vThr->dec)
    {
        if (vThr->lock())
        {
            vThr->destroyDec();
            vThr->destroySubtitlesDecoder();
        }
        else
        {
            vThr->stop(true);
            vThr = nullptr;
        }
    }
    frame_last_pts = frame_last_delay = 0.0;
    subtitlesStream = -1;
    nextFrameB = false;
    // delete ass; //wywołuje też closeASS(), sMutex nie potrzebny, bo vThr jest zablokowany (mutex przed sMutex)
    //ass = nullptr;
    fps = 0.0;
}
void LPlayer::stopADec()
{
    if (aThr && aThr->dec)
    {
        if (aThr->lock())
            aThr->destroyDec();
        else
        {
            aThr->stop(true);
            aThr = nullptr;
        }
    }
    audio_current_pts = skipAudioFrame = audio_last_delay = 0.0;
}
