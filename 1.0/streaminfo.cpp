#include "streaminfo.hpp"

StreamInfo::StreamInfo()
{

    audioStream_ = nullptr;
    videoStream_ = nullptr;
    audioCodeContex_ = nullptr;           //音频解码上下文
    videoCodeContex_ = nullptr;
    pVideoCodec_ = nullptr;
    pAudioCodec_ = nullptr;


}

StreamsInfo::~StreamsInfo()
{
    for (int i = 0; i < count(); ++i)
        delete at(i);
}
