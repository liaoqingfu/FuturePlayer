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
