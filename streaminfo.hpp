#ifndef STREAMINFO_HPP
#define STREAMINFO_HPP
#include <QByteArray>

class StreamInfo
{
public:
    StreamInfo();
    QByteArray codec_name, title, artist, format;
};

#endif // STREAMINFO_HPP
