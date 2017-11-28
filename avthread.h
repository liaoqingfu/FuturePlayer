#ifndef AVTHREAD_H
#define AVTHREAD_H
#include <QCoreApplication>
#include <QStringList>
#include <QThread>
#include <QMutex>

class AVThread: public QThread
{
    Q_OBJECT
public:
    AVThread();
};

#endif // AVTHREAD_H
