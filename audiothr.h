#ifndef AUDIOTHR_H
#define AUDIOTHR_H
#include <QVector>
#include <QStringList>
#include "avthread.hpp"
#include <SndResampler.hpp>
class AudioFilter;

class AudioThr : public AVThread
{
   // Q_OBJECT
public:
    AudioThr(LPlayer &player, const QStringList &pluginsName);
    ~AudioThr();

    void stop(bool terminate = false) override final;
    void clearVisualizations();

    bool setParams(uchar realChn, uint realSRate, uchar chn = 0, uint sRate = 0);

    void silence(bool invert = false);

    inline void setAllowAudioDrain()
    {
        allowAudioDrain = true;
    }
private:
    void run() override final;

    bool resampler_create();

#ifdef Q_OS_WIN
    void timerEvent(QTimerEvent *) override final;
#endif

    SndResampler sndResampler;
    uchar realChannels, channels;
    uint  realSample_rate, sample_rate;
    double lastSpeed;

    int tmp_br;
    double tmp_time, silence_step;
    volatile double doSilence;
    QMutex silenceChMutex;
#ifdef Q_OS_WIN
    bool canUpdatePos, canUpdateBitrate;
#endif
    bool allowAudioDrain;

    QVector<AudioFilter *> filters;
private slots:
    void pauseVis(bool);
signals:
    void pauseVisSig(bool);
};

#endif // AUDIOTHR_H
