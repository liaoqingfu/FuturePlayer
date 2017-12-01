#include "ffdemux.h"
#include "formatcontext.h"
#include "demuxerfactory.hpp"

#include <qDebug>

//FFDemux::FFDemux(QMutex &avcodec_mutex) :
//    avcodec_mutex(avcodec_mutex),
//    abortFetchTracks(false),
//    reconnectStreamed(false)
//{

//}

FFDemux::FFDemux()
{
    qDebug() << "FFDemux init";
}
qint64 FFDemux::size() const
{
    qint64 bytes = -1;
    for (const FormatContext *fmtCtx : formatContexts)
    {
        const qint64 s = fmtCtx->size();
        if (s < 0)
            return -1;
        bytes += s;
    }
    return bytes;
}
double FFDemux::length() const
{
    double length = -1.0;
    for (const FormatContext *fmtCtx : formatContexts)
        length = qMax(length, fmtCtx->length());
    return length;
}
int FFDemux::bitrate() const
{
    int bitrate = 0;
    for (const FormatContext *fmtCtx : formatContexts)
        bitrate += fmtCtx->bitrate();
    return bitrate;
}
QByteArray FFDemux::image(bool forceCopy) const
{
    if (formatContexts.count() == 1)
        return formatContexts.at(0)->image(forceCopy);
    return QByteArray();
}

bool FFDemux::localStream() const
{
    for (const FormatContext *fmtCtx : formatContexts)
        if (!fmtCtx->isLocal)
            return false;
    return true;
}

bool FFDemux::seek(int pos, bool backward)
{
    bool seeked = false;
    for (FormatContext *fmtCtx : formatContexts)
    {
        if (fmtCtx->seek(pos, backward))
            seeked |= true;
        else if (fmtCtx->isStreamed && formatContexts.count() > 1)
        {
            fmtCtx->setStreamOffset(pos);
            seeked |= true;
        }
    }
    return seeked;
}
bool FFDemux::read(Packet &encoded, int &idx)
{
    int fmtCtxIdx = -1;
    int numErrors = 0;

    double ts;
    for (int i = 0; i < formatContexts.count(); ++i)
    {
        FormatContext *fmtCtx = formatContexts.at(i);
        if (fmtCtx->isError)
        {
            ++numErrors;
            continue;
        }
        if (fmtCtxIdx < 0 || fmtCtx->currPos < ts)
        {
            ts = fmtCtx->currPos;
            fmtCtxIdx = i;
        }
    }

    if (fmtCtxIdx < 0) //Every format context has an error
        return false;

    if (formatContexts.at(fmtCtxIdx)->read(encoded, idx))
    {
        for (int i = 0; i < fmtCtxIdx; ++i)
            idx += formatContexts.at(i)->streamsInfo.count();
        return true;
    }

    return numErrors < formatContexts.count() - 1; //Not Every format context has an error
}
void FFDemux::pause()
{
    for (FormatContext *fmtCtx : formatContexts)
        fmtCtx->pause();
}
void FFDemux::abort()
{
    QMutexLocker mL(&mutex);
    for (FormatContext *fmtCtx : formatContexts)
        fmtCtx->abort();
    abortFetchTracks = true;
}

bool FFDemux::open(const QString &entireUrl)
{
    QString prefix, url, param;

    {
        if (!param.isEmpty())
            addFormatContext(url, param);
        else for (QString stream : url.split("][", QString::SkipEmptyParts))
        {
            stream.remove('[');
            stream.remove(']');
            addFormatContext(stream);
        }
    }
    return !formatContexts.isEmpty();
}

void FFDemux::addFormatContext(QString url, const QString &param)
{

}
