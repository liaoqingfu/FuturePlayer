#include "avthread.hpp"

AVThread::AVThread(LPlayer &player, const QString &writer_type, Writer *_writer, const QStringList &pluginsName) :
    dec(nullptr), playC(player), br(false), br2(false), waiting(false),
    mutex(QMutex::Recursive)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

    if (_writer)
        writer = _writer;


    mutex.lock();

    if (writer)
        start();
}

AVThread::~AVThread()
{

}

void AVThread::destroyDec()
{
    delete dec;
    dec = nullptr;
}

bool AVThread::lock()
{
    br2 = true;
    if (!mutex.tryLock(MUTEXWAIT_TIMEOUT))
    {
        const bool ret = mutex.tryLock(MUTEXWAIT_TIMEOUT * 2);
        if (!ret)
        {
            br2 = false;
            return false;
        }
    }
    return true;
}

void AVThread::unlock()
{
    br2 = false;
    mutex.unlock();
}

void AVThread::stop(bool _terminate)
{
    if (_terminate)
        return terminate();

    br = true;
    mutex.unlock();

    if (!wait(TERMINATE_TIMEOUT))
        terminate();
}

bool AVThread::hasDecoderError() const
{
    return false;
}

void AVThread::terminate()
{
    disconnect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    QThread::terminate();
    wait(1000);
    deleteLater();
}
