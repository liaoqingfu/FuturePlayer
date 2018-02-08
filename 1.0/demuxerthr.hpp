#ifndef DEMUXERTHR_H
#define DEMUXERTHR_H

#include <QThread>

#include <QString>
#include <QThread>
#include <QMutex>
#include <QTimer>

class BufferInfo;
class LPlayer;
class AVThread;
class Demuxer;
class BasicIO;

class DemuxerThr : public QThread
{
    friend class DemuxerTimer;
    friend class LPlayer;
    Q_OBJECT
private:
    DemuxerThr(LPlayer &);
    ~DemuxerThr() final;


    QByteArray getCoverFromStream() const;

    inline bool isDemuxerReady() const
    {
        return demuxerReady;
    }
    inline bool canSeek() const
    {
        return !unknownLength;
    }

    void seek(bool doDemuxerSeek);

    void stop();
    void end();

    void emitInfo();

    bool load(bool canEmitInfo = true);

    void checkReadyWrite(AVThread *avThr);

    void run() override final;

    inline void ensureTrueUpdateBuffered();
    inline bool canUpdateBuffered() const;
    void handlePause();
    void emitBufferInfo(bool clearBackwards);

    void updateCoverAndPlaying();

    bool mustReloadStreams();
    bool bufferedAllPackets(int vS, int aS, int p);
    bool emptyBuffers(int vS, int aS);
    bool canBreak(const AVThread *avThr1, const AVThread *avThr2);
    double getAVBuffersSize(int &vS, int &aS);
    BufferInfo getBufferInfo(bool clearBackwards);
    void clearBuffers();

    double getFrameDelay() const;

    LPlayer &playC;

    QString name, url, updatePlayingName;

    int minBuffSizeLocal, minBuffSizeNetwork;
    bool err, updateBufferedSeconds, demuxerReady, hasCover, skipBufferSeek, localStream, unknownLength, waitingForFillBufferB, paused, demuxerPaused;
    QMutex stopVAMutex, endMutex, seekMutex;
    Demuxer *demuxer;
    QString title, artist, album;
    double playIfBuffered, time, updateBufferedTime;
private slots:
    void stopVADec();
    void updateCover(const QString &title, const QString &artist, const QString &album, const QByteArray &cover);
signals:
    void load(Demuxer *);
};

/**/

class DemuxerTimer : public QObject
{
    Q_OBJECT
public:
    DemuxerTimer(DemuxerThr &demuxerThr);

    inline void start();
    inline void stop();
private slots:
    void timeout();
private:
    DemuxerThr &demuxerThr;
    QTimer t;
};

#endif // DEMUXERTHR_H
