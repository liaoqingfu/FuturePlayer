#ifndef DEMUXER_H
#define DEMUXER_H

#include "packet.h"
#include <QByteArray>
class Demuxer
{
public:
    virtual qint64 size() const;
    virtual double length() const = 0;
    virtual int bitrate() const = 0;
    virtual QByteArray image(bool forceCopy = false) const;

    virtual bool localStream() const;
    virtual bool dontUseBuffer() const;

    virtual bool seek(int pos, bool backward) = 0;
    virtual bool read(Packet &, int &) = 0;
    virtual void pause(){}
    virtual void abort(){}
    virtual bool open(const QString &url) = 0;
};

#endif // DEMUX_H
