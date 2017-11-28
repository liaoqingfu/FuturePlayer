#ifndef FFCOMMON_H
#define FFCOMMON_H

#define PLAYER_NOPTS_VALUE ((qint64)AV_NOPTS_VALUE)

struct AVDictionary;
struct AVPacket;

class FFCommon
{
public:
    FFCommon();
    static AVPacket *createAVPacket();
    static void freeAVPacket(AVPacket *&packet);
};

#endif // FFCOMMON_H
