#include "demuxerfactory.hpp"



extern "C" Demuxer* DemuxerCreate(const std::string& strKey)
{
    //return DemuxerFactory::Instance().Create(strKey);
    return nullptr;
}
