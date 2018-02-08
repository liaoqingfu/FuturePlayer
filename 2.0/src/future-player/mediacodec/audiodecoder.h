#ifndef AUDIODECODER_H
#define AUDIODECODER_H


namespace Darren
{
class AudioDecoder
{
public:
    AudioDecoder() {}
    virtual ~AudioDecoder() {}
    virtual int init(const int withHead){}
    virtual int init(const int withHead, const unsigned char *header, int headerLength){  }
    virtual int inputData(const unsigned char *inputData, int inputDataLength, unsigned char *outputData, int &outputDataLength){}

    virtual int inputData();
    virtual unsigned char getChannels() const
    {
        return channels_;
    }

    virtual unsigned int getSampleRate() const
    {
        return sampleRate_;
    }

    virtual unsigned char getSampleBit() const
    {
        return sampleBit_;
    }
    virtual unsigned char getBitRate() const
    {
        return bitRate_;
    }

protected:
    unsigned int sampleRate_;
    unsigned int bitRate_;
    unsigned char channels_;
    unsigned char sampleBit_;
};
}


#endif // AUDIODECODER_H
