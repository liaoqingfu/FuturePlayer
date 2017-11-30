#ifndef AUDIOFILTER_HPP
#define AUDIOFILTER_HPP
#include <QVector>
class  Buffer;
class AudioFilter
{
public:
    virtual ~AudioFilter() = default;

    static QVector<AudioFilter *> open();

    virtual bool setAudioParameters(uchar chn, uint srate) = 0;
    virtual int bufferedSamples() const;
    virtual void clearBuffers();
    virtual double filter(Buffer &data, bool flush = false) = 0; //returns delay in [s]
};

#endif // AUDIOFILTER_HPP
