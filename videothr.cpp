#include "videothr.h"
#include "player.h"
#include "writer.hpp"

VideoThr::VideoThr(LPlayer &playC, Writer *hwAccelWriter, const QStringList &pluginsName) :
    AVThread(playC, "video:", hwAccelWriter, pluginsName)
{

}

VideoThr::~VideoThr()
{

}
