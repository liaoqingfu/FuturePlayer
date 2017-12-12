#ifndef FORMATCONTEXT_H
#define FORMATCONTEXT_H

#include <QMutex>
#include <QVector>
#include "packet.h"
#include "streaminfo.hpp"
#include "openthr.hpp"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/replaygain.h"
#include "libavutil/pixdesc.h"
#include "SDL.h"
#include "SDL_thread.h"
}


class FormatContext
{
public:
    FormatContext(QMutex &avcodec_mutex, bool reconnectStreamed = false);
    ~FormatContext();
    QString name() const;
    QString title() const;
    qint64 size() const;
    double length() const;
    int bitrate() const;

    bool seek(double pos, bool backward);
    bool read(Packet &encoded, int &idx);
    void pause();
    void abort();

    bool open(const QString &_url, const QString &param = QString());

    void setStreamOffset(double offset);
    QByteArray image(bool forceCopy) const;
    bool isLocal, isStreamed, isError;
    StreamsInfo streamsInfo;
    double currPos;
private:
    QSharedPointer<AbortContext> abortCtx;
    QVector<int> index_map;
    QVector<AVStream *> streams;
    QVector<TimeStamp> streamsTS;
    QVector<double> streamsOffset;
    QVector<double> nextDts;
    AVFormatContext *formatCtx;
    AVPacket *packet;

    bool reconnectStreamed;
    bool isPaused, fixMkvAss;
    mutable bool isMetadataChanged; //被mutable修饰的变量，将永远处于可变的状态，即使在一个const函数中。
    double lastTime, startTime;

    int invalErrCount, errFromSeek;
    bool maybeHasFrame;
    bool stillImage;
    QMutex &avcodec_mutex;
};

#endif // FORMATCONTEXT_H
