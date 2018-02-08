#ifndef AVTHREAD_H
#define AVTHREAD_H
#include <QCoreApplication>
#include <QStringList>
#include <QThread>
#include <QMutex>
#include <QStringList>
#include "decoder.h"
#include "writer.hpp"
#define MUTEXWAIT_TIMEOUT 1250
#define TERMINATE_TIMEOUT (MUTEXWAIT_TIMEOUT*3)

class LPlayer;
class AVThread: public QThread
{
public:
    void destroyDec();
    inline void setDec(Decoder *_dec)
    {
        dec = _dec;
    }

    inline bool updateTryLock()
    {
        return updateMutex.tryLock();
    }
    inline void updateUnlock()
    {
        updateMutex.unlock();
    }

    bool lock();
    void unlock();

    inline bool isWaiting() const
    {
        return waiting;
    }

    virtual void stop(bool terminate = false);

    virtual bool hasDecoderError() const;

    Decoder *dec;
    Writer *writer;
protected:
    AVThread(LPlayer &player, const QString &, Writer *writer = nullptr, const QStringList &pluginsName = {});
    virtual ~AVThread();

    void terminate();

    LPlayer &playC;

    volatile bool br, br2;
    bool waiting;
    QMutex mutex, updateMutex;
};

#endif // AVTHREAD_H
