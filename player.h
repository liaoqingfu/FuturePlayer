#ifndef PLAYER_H
#define PLAYER_H
#include "demuxerthr.h"
#include "videothr.h"
#include "audiothr.h"

/*******************************************
 *
 * 播放器的统一接口
 ******************************************/
class Player
{
public:
    Player();
    void play(const QString &);             // 播放
    void stop(bool quitApp = false);        // 停止
    void restart();
    inline bool canUpdatePosition() const
    {
        return canUpdatePos;
    }

    void chPos(double, bool updateGUI = true);

    void togglePause();
    void seek(double pos, bool allowAccurate = true);
    void chStream(const QString &s);
    void setSpeed(double);

    bool isPlaying() const;

    inline QString getUrl() const
    {
        return url;
    }
    inline double getPos() const
    {
        return pos;
    }
private:
    DemuxerThr *demuxThr;
    VideoThr *vThr;
    AudioThr *aThr;

    QString url, newUrl;
    bool canUpdatePos;
    double pos;
};

#endif // PLAYER_H
