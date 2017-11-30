#include "streaminfo.hpp"

StreamInfo::StreamInfo()
{

}

StreamsInfo::~StreamsInfo()
{
    for (int i = 0; i < count(); ++i)
        delete at(i);
}
