#ifndef FFDEMUX_H
#define FFDEMUX_H

#include "demuxer.h"

#include "packet.h"
#include <QMutex>
#include <QVector>

class FormatContext;

class FFDemux : public Demuxer
{
public:
    FFDemux();
    //FFDemux(QMutex &);
    qint64 size() const override final;
    double length() const override final;
    int bitrate() const override final;
    QByteArray image(bool forceCopy) const override final;

    bool localStream() const override final;

    bool seek(int pos, bool backward) override final;
    bool read(Packet &encoded, int &idx) override final;
    void pause() override final;
    void abort() override final;

    bool open(const QString &entireUrl) override final;

    /**/

    void addFormatContext(QString entireUrl, const QString &param = QString());

    QVector<FormatContext *> formatContexts;

    //QMutex &avcodec_mutex;
    QMutex mutex;

    bool abortFetchTracks;
    bool reconnectStreamed;
};

#endif // FFDEMUX_H
