#include "audiofilter.hpp"

QVector<AudioFilter *> AudioFilter::open()
{
    QVector<AudioFilter *> filterList;

    filterList.squeeze();
    return filterList;
}

int AudioFilter::bufferedSamples() const
{
    return 0;
}
void AudioFilter::clearBuffers()
{}
