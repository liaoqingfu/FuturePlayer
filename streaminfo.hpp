#ifndef STREAMINFO_HPP
#define STREAMINFO_HPP
#include <QByteArray>
#include <QList>
class StreamInfo
{
public:
    StreamInfo();
    QByteArray codec_name, title, artist, format;
};


class StreamsInfo : public QList<StreamInfo *>
{
    Q_DISABLE_COPY(StreamsInfo)
public:
    StreamsInfo() = default;
    ~StreamsInfo();
};
#endif // STREAMINFO_HPP
