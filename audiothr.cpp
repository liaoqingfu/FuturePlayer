#include "audiothr.h"
#include "player.h"

AudioThr::AudioThr(LPlayer &player, const QStringList &pluginsName) :
     AVThread(player, "audio:", nullptr, pluginsName)
{

}
AudioThr::~AudioThr()
{

}

