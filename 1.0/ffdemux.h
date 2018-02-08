#ifndef FFDEMUX_H
#define FFDEMUX_H

#include "demuxer.h"

#include "packet.h"
#include <QMutex>
#include <QVector>
#include "streaminfo.hpp"
class FormatContext;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/replaygain.h"
#include "libavutil/pixdesc.h"
}



class FFDemux : public Demuxer
{
public:
//    FFDemux();
    FFDemux(StreamInfo &streamInfo);
    inline void freeCommSpace();
    //FFDemux(QMutex &);
    qint64 size() const override final;
    double length() const override final;
    int bitrate() const override final;
    QByteArray image(bool forceCopy) const override final;

    bool localStream() const override final;

    bool seek(int pos, bool backward) override final;
    bool read(Packet &encoded, Decoder::CodeType &codeType) override final;
    void pause() override final;
    void abort() override final;
    bool isEof() override final;

    bool open(const QString &entireUrl) override final;
    bool init(StreamInfo &streamInfo);
    /**/

    void addFormatContext(QString entireUrl, const QString &param = QString());
    inline int64_t getDuration();
    QVector<FormatContext *> formatContexts;

    //QMutex &avcodec_mutex;
    QMutex mutex;

    bool abortFetchTracks;
    bool reconnectStreamed;
    AVFormatContext* formatCtx_;    //格式上下文
    AVInputFormat *inputFmt_;       // 如果提前获知文件格式可以直接设置进去，避免反复地探测
    QString entireUrl_;             // 保存播放文件的url
    AVDictionary *dictOptions_;        //
//    对于  const类型和&引用类型数据成员，其初始化必须在初始化阶段完成。若通过
//    普通计算阶段来初始化该值，编译器会报错：该变量未初始化。
    StreamInfo &streamInfo_;

    AVPacket *packet_;
    double lastTime_;
    double startTime_;
    bool isEof_;
};

#endif // FFDEMUX_H
