#ifndef STREAMINFO_HPP
#define STREAMINFO_HPP
#include <QByteArray>
#include <QList>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/replaygain.h"
#include "libavutil/pixdesc.h"
}

class StreamInfo
{
public:
    StreamInfo();
    AVCodec *getAudioDecoder()
    {
        return pAudioCodec_;
    }
    AVCodec *getVideoDecoder()
    {
        return pVideoCodec_;
    }

    QByteArray codec_name, title, artist, format;
    int audioIndex_;
    int videoIndex_;
    AVStream *audioStream_;
    AVStream *videoStream_;
    AVCodecContext *audioCodeContex_;           //音频解码上下文
    AVCodecContext *videoCodeContex_;           //视频解码上下文
    AVCodec* pAudioCodec_;
    AVCodec* pVideoCodec_;
};


class StreamsInfo : public QList<StreamInfo *>
{
    Q_DISABLE_COPY(StreamsInfo)
public:
    StreamsInfo() = default;
    ~StreamsInfo();
};
#endif // STREAMINFO_HPP
