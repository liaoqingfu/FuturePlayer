#include "demuxer.h"

QByteArray Demuxer::image(bool forceCopy) const
{
    Q_UNUSED(forceCopy)
    return QByteArray();
}
qint64 Demuxer::size() const
{
    return -1;
}
bool Demuxer::localStream() const
{
    return true;
}
bool Demuxer::dontUseBuffer() const
{
    return false;
}
