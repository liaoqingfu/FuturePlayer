#include "videothr.hpp"
#include "player.hpp"
#include "writer.hpp"

VideoThr::VideoThr(LPlayer &playC, Writer *hwAccelWriter, const QStringList &pluginsName) :
    AVThread(playC, "video:", hwAccelWriter, pluginsName)
{

}

VideoThr::~VideoThr()
{

}
void VideoThr::run()
{

}

void VideoThr::stop(bool terminate)
{
    playC.videoSeekPos = -1;
    AVThread::stop(terminate);
}

void VideoThr::destroySubtitlesDecoder()
{
    deleteSubs = true;
    if (sDec)
    {
        delete sDec;
        sDec = nullptr;
    }
}
