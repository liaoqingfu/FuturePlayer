#ifndef VIDEOTHR_H
#define VIDEOTHR_H

#include "avthread.hpp"


class VideoThr : public AVThread
{
public:
    VideoThr(LPlayer &, Writer *, const QStringList &pluginsName = {});
    ~VideoThr();
};

#endif // VIDEOTHR_H
