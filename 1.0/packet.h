#ifndef PACKET_H
#define PACKET_H

#include <TimeStamp.h>
#include <buffer.hpp>

class Packet : public Buffer
{
public:
    TimeStamp ts;
    double duration;            // 单位为秒
    double sampleAspectRatio = 0.0;
    bool hasKeyFrame = true;
};
#endif // PACKET_H
