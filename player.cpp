#include "player.hpp"
#include <QDebug>

LPlayer::LPlayer():
    demuxThr(nullptr), vThr(nullptr), aThr(nullptr)
{
    videoEnabled = true;
    audioEnabled = true;
    subtitlesEnabled = true;
}

void LPlayer::play(const QString &_url)
{
    qDebug() << "url = " << _url.toStdString().c_str();
    // 创建Demuxer线程
    if (!demuxThr)
    {
        if (audioEnabled || videoEnabled)
        {
            audioStream = videoStream = subtitlesStream = -1;

            url = _url;
            demuxThr = new DemuxerThr(*this);
            demuxThr->start();
            state_ = PlayingState;
        }
    }
}

void LPlayer::stop(bool _quitApp)
{
    qDebug() << "LPlayer::stop entry";
    if(demuxThr)
    {

        demuxThr->stop();
        qDebug() << "LPlayer::stop entry2";
        delete demuxThr;
        qDebug() << "LPlayer::stop entry3";
        demuxThr = nullptr;
        state_ = StoppedState;
    }
    qDebug() << "LPlayer::stop exit";
}

LPlayer::State LPlayer::state()
{
    return state_;
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
void LPlayer::setVolume(int volume)
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


bool LPlayer::initAudioThr()
{
    if(!aThr)
    {
        //aThr = new
    }
    return true;
}
bool LPlayer::initVideoThr()
{
    return true;
}

void LPlayer::setAudioDecoder(Decoder *dec)
{
    aThr->setDec(dec);
}
void LPlayer::setVideoDecoder(Decoder *dec)
{
    vThr->setDec(dec);
}
void LPlayer::startAudioThr()
{
    aThr->start();
}
void LPlayer::startVideoThr()
{
    vThr->start();
}
