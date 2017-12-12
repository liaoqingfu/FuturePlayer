#include "demuxerthr.hpp"
#include "player.hpp"
#include "demuxer.h"
#include "streaminfo.hpp"
#include "demuxerfactory.hpp"



/* DemuxerThr */

#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <cmath>

static inline bool getCurrentPlaying(int stream, const QList<StreamInfo *> &streamsInfo, const StreamInfo *streamInfo)
{
    return (stream > -1 && streamsInfo.count() > stream) && streamsInfo[stream] == streamInfo;
}
static QString getWriterName(AVThread *avThr)
{
    QString decName;
    if (avThr)
    {
        decName = avThr->dec->name();
        if (!decName.isEmpty())
            decName += ", ";
    }
    return (avThr && avThr->writer) ? " - <i>" + decName + avThr->writer->name() + "</i>" : QString();
}

static QString getCoverFile(const QString &title, const QString &artist, const QString &album)
{
    return  QString("test");
}

class BufferInfo
{
public:
    inline BufferInfo() :
        remainingDuration(0.0), backwardDuration(0.0),
        firstPacketTime(-1), lastPacketTime(-1),
        remainingBytes(0), backwardBytes(0)
    {}

    double remainingDuration, backwardDuration;
    qint32 firstPacketTime, lastPacketTime;
    qint64 remainingBytes, backwardBytes;
};


DemuxerThr::DemuxerThr(LPlayer &playC) :
    playC(playC),
    err(false), demuxerReady(false), hasCover(false),
    skipBufferSeek(false), localStream(true), unknownLength(true), waitingForFillBufferB(false), paused(false), demuxerPaused(false),
    updateBufferedTime(0.0)
{
    url = playC.getUrl();
}
DemuxerThr::~DemuxerThr()
{}

QByteArray DemuxerThr::getCoverFromStream() const
{
    return demuxer ? demuxer->image(true) : QByteArray();
}

void DemuxerThr::seek(bool doDemuxerSeek)
{
    if (!doDemuxerSeek && skipBufferSeek)
        return;
    if (playC.seekTo >= 0 || (playC.seekTo == SEEK_STREAM_RELOAD || playC.seekTo == SEEK_REPEAT))
    {
        AVThread *aThr = (AVThread *)playC.aThr, *vThr = (AVThread *)playC.vThr;

        //emit playC.chText(tr("Seeking"));
        playC.canUpdatePos = false;

        bool repeat = false;

        bool seekInBuffer = !skipBufferSeek;
        if (playC.seekTo == SEEK_STREAM_RELOAD) //po zmianie strumienia audio, wideo lub napisów lub po ponownym uruchomieniu odtwarzania
        {
            playC.seekTo = playC.pos;
            seekInBuffer = false;
        }
        else if (playC.seekTo == SEEK_REPEAT)
        {
            playC.seekTo = 0;
            repeat = true;
        }


        const Qt::CheckState accurateSeek = Qt::Unchecked;
        const bool doAccurateSeek = (accurateSeek == Qt::Checked || (accurateSeek == Qt::PartiallyChecked && (!localStream || playC.allowAccurateSeek)));

        const bool backward = doAccurateSeek || repeat || (playC.seekTo < (int)playC.pos);
        bool flush = false, aLocked = false, vLocked = false;

        skipBufferSeek = false;

        bool cantSeek = false;

        if (seekInBuffer && (!localStream || !backward))
        {
            playC.vPackets.lock();
            playC.aPackets.lock();
            playC.sPackets.lock();

            const auto doSeekInBuffer = [&] {
                double seekTo = playC.seekTo;
                bool ok = (!playC.vPackets.isEmpty() || !playC.aPackets.isEmpty());

                if (ok && !playC.vPackets.isEmpty())
                {
                    ok &= playC.vPackets.seekTo(seekTo, backward);
                    if (ok)
                        seekTo = playC.vPackets.currentPacketTime(); // Use video position for audio seeking
                }

                if (ok && !playC.aPackets.isEmpty())
                    ok &= playC.aPackets.seekTo(seekTo, backward);

                if (ok && !playC.sPackets.isEmpty())
                    ok &= playC.sPackets.seekTo(seekTo, backward);

                return ok;
            };

            if (doSeekInBuffer())
            {
                doDemuxerSeek = false;
                flush = true;
                ensureTrueUpdateBuffered();
                if (aThr)
                    aLocked = aThr->lock();
                if (vThr)
                    vLocked = vThr->lock();
            }
            else
            {
                if (!accurateSeek && !backward && !localStream && playC.endOfStream)
                {
                    // Don't seek in demuxer on network streams if we can't seek in buffer
                    // and we have end of stream which means that everything is buffered.
                    doDemuxerSeek = false;
                    cantSeek = true;
                }
                else
                {
                    //emit playC.updateBufferedRange(-1, -1);
                    updateBufferedTime = 0.0;
                }
            }

            playC.vPackets.unlock();
            playC.aPackets.unlock();
            playC.sPackets.unlock();
        }

        if (doDemuxerSeek && demuxer->seek(playC.seekTo, backward))
            flush = true;
        else if (!doDemuxerSeek && !flush && !cantSeek)
        {
            skipBufferSeek = true;
            if (!localStream && !unknownLength)
                demuxer->abort(); //Abort only the Demuxer, not IOController
        }
        else if (repeat && doDemuxerSeek)
        {
            playC.seekTo = SEEK_REPEAT; //Notify that repeat seek failed
        }

        if (flush)
        {
            playC.endOfStream = false;
            if (doDemuxerSeek)
                clearBuffers();

            if (!aLocked && aThr)
                aLocked = aThr->lock();
            if (!vLocked && vThr)
                vLocked = vThr->lock();
            playC.skipAudioFrame = playC.audio_current_pts = 0.0;
            playC.flushVideo = playC.flushAudio = true;
            if (playC.pos < 0.0) //skok po rozpoczęciu odtwarzania po uruchomieniu programu
            {

                //emit playC.updatePos(playC.seekTo); //uaktualnia suwak na pasku do wskazanej pozycji
            }
            const int seekPos = (doAccurateSeek && playC.seekTo > 0) ? playC.seekTo : -1;
            if (vThr)
                playC.videoSeekPos = seekPos;
            if (aThr)
                playC.audioSeekPos = seekPos;
            if (aLocked)
                aThr->unlock();
            if (vLocked)
                vThr->unlock();
        }

        if (!skipBufferSeek)
        {
            playC.canUpdatePos = true;
            if (playC.seekTo != SEEK_REPEAT) //Don't reset variable if repeat seek failed
                playC.seekTo = SEEK_NOWHERE;
            if (!playC.paused)
            {
                //emit playC.chText(tr("Playback"));
            }
            else
                playC.paused = false;
        }
    }
}

void DemuxerThr::stopVADec()
{
    clearBuffers();

    playC.stopVDec();
    playC.stopADec();

    stopVAMutex.unlock();

    endMutex.lock(); //Czeka do czasu zniszczenia demuxer'a - jeżeli wcześniej mutex był zablokowany (wykonał się z wątku)
    endMutex.unlock(); //odblokowywuje mutex
}

void DemuxerThr::run()
{
    demuxer =  DemuxerCreate("FFDemux");
    qDebug() << "DemuxerThr::run() lily";
}

void DemuxerThr::stop()
{

    //demuxer->abort();
}

void DemuxerThr::updateCover(const QString &title, const QString &artist, const QString &album, const QByteArray &cover)
{
    const QImage coverImg = QImage::fromData(cover);
    if (!coverImg.isNull())
    {
        if (this->title == title && this->artist == artist && (this->album == album || (album.isEmpty() && !title.isEmpty())))
        {
            //emit playC.updateImage(coverImg);
        }
        QDir dir("QMPlay2Core.getSettingsDir()");
        dir.mkdir("Covers");
        QFile f(getCoverFile(title, artist, album));
        if (f.open(QFile::WriteOnly))
        {
            f.write(cover);
            f.close();
        }
    }
}


void DemuxerThr::emitBufferInfo(bool clearBackwards)
{

}

inline void DemuxerThr::ensureTrueUpdateBuffered()
{
    time = 0;// Functions::gettime() - updateBufferedTime; //zapewni, że updateBuffered będzie na "true";
}
inline bool DemuxerThr::canUpdateBuffered() const
{
    return true;// Functions::gettime() - time >= updateBufferedTime;
}
void DemuxerThr::handlePause()
{

}

void DemuxerThr::clearBuffers()
{
    playC.vPackets.clear();
    playC.aPackets.clear();
    //playC.clearSubtitlesBuffer();
}


/* BufferInfoTimer */

DemuxerTimer::DemuxerTimer(DemuxerThr &demuxerThr) :
    demuxerThr(demuxerThr)
{
    connect(&t, SIGNAL(timeout()), this, SLOT(timeout()));
}

inline void DemuxerTimer::start()
{
    t.start(500);
}
inline void DemuxerTimer::stop()
{
    t.stop();
}

void DemuxerTimer::timeout()
{
    demuxerThr.handlePause();
    if (demuxerThr.canUpdateBuffered())
        demuxerThr.emitBufferInfo(true);
}
