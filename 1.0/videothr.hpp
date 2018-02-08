#ifndef VIDEOTHR_H
#define VIDEOTHR_H

#include "avthread.hpp"
#include "pixelformat.hpp"
#include "videowriter.hpp"
#include "videofilters.hpp"
#include "playerosd.hpp"
#include "videoframe.hpp"

class VideoThr : public AVThread
{
public:
    VideoThr(LPlayer &, Writer *, const QStringList &pluginsName = {});
    ~VideoThr() final;

    void stop(bool terminate = false) override final;

    inline VideoWriter *getHWAccelWriter() const
    {
        return hwAccelWriter;
    }

    QMPlay2PixelFormats getSupportedPixelFormats() const;

    inline void setDoScreenshot()
    {
        doScreenshot = true;
    }
    inline void setSyncVtoA(bool b)
    {
        syncVtoA = b;
    }
    inline void setDeleteOSD()
    {
        deleteOSD = true;
    }

    void destroySubtitlesDecoder();
    inline void setSubtitlesDecoder(Decoder *dec)
    {
        sDec = dec;
    }

    bool setSpherical();
    bool setFlip();
    bool setRotate90();
    void setVideoAdjustment();
    void setFrameSize(int w, int h);
    void setARatio(double aRatio, double sar);
    void setZoom();

    void initFilters(bool processParams = true);

    bool processParams();

    void updateSubs();

private:
    inline VideoWriter *videoWriter() const;

    void run() override final;

    bool deleteSubs, syncVtoA, doScreenshot, canWrite, deleteOSD, deleteFrame, gotFrameOrError;
    double lastSampleAspectRatio;
    int W, H;
    quint32 seq;

    Decoder *sDec;
    VideoWriter *hwAccelWriter;
    PlayerOSD *subtitles;
    VideoFilters filters;
    QMutex filtersMutex;
private slots:
    void write(VideoFrame videoFrame, quint32 lastSeq);
    void screenshot(VideoFrame videoFrame);
    void pause();
};

#endif // VIDEOTHR_H
