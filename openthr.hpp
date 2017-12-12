#ifndef OPENTHR_HPP
#define OPENTHR_HPP


#include <QSharedPointer>
#include <QWaitCondition>
#include <QThread>
#include <QMutex>

class AbortContext
{
public:
    void abort();

    QWaitCondition openCond;
    QMutex openMutex;
    bool isAborted = false;
};

/**/

struct AVFormatContext;
struct AVInputFormat;
struct AVDictionary;

class OpenThr : public QThread
{
public:
    void drop();

protected:
    OpenThr(const QByteArray &url, AVDictionary *options, QSharedPointer<AbortContext> &abortCtx);

    bool waitForOpened() const;

    bool wakeIfNotAborted();

    const QByteArray m_url;
    AVInputFormat *m_inputFmt;
    AVDictionary *m_options;

    QSharedPointer<AbortContext> m_abortCtx;

private:
    bool m_finished;
};
#endif // OPENTHR_HPP
