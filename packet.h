#ifndef PACKET_H
#define PACKET_H

#include <TimeStamp.h>
#include <Buffer.h>

class Packet : public Buffer
{
public:
    TimeStamp ts;
    double duration, sampleAspectRatio = 0.0;
    bool hasKeyFrame = true;
};
#endif // PACKET_H
