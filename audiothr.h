#ifndef AUDIOTHR_H
#define AUDIOTHR_H
#include <QStringList>
#include "avthread.hpp"


class AudioThr : public AVThread
{
   // Q_OBJECT
public:
    AudioThr(LPlayer &player, const QStringList &pluginsName);
    ~AudioThr();
};

#endif // AUDIOTHR_H
