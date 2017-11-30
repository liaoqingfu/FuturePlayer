#ifndef PLAYER_H
#define PLAYER_H

#include <QTimer>
#include "demuxerthr.h"
#include "videothr.hpp"
#include "audiothr.h"

/*******************************************
 *
 * 播放器的统一接口
 ******************************************/
class LPlayer
{
public:
    LPlayer();
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
    void loadSubsFile(const QString &fileName);
    inline QString getUrl() const
    {
        return url;
    }
    inline double getPos() const
    {
        return pos;
    }

    double frame_last_pts, frame_last_delay, audio_current_pts, audio_last_delay;
    bool doSilenceOnStart, canUpdatePos, paused, waitForData, flushVideo, flushAudio, muted, reload, nextFrameB, endOfStream, ignorePlaybackError;
    int seekTo, lastSeekTo, restartSeekTo, seekA, seekB, videoSeekPos, audioSeekPos;
    double vol[2], replayGain, zoom, pos, skipAudioFrame, videoSync, speed, subtitlesSync, subtitlesScale;
    int flip;
    bool rotate90, spherical, stillImage;


private:
    void stopVThr();
    void stopAThr();
    inline void stopAVThr();

    void stopVDec();
    void stopADec();

private:
    DemuxerThr *demuxThr;
    VideoThr *vThr;
    AudioThr *aThr;

    QString url, newUrl, aRatioName;
    int audioStream, videoStream, subtitlesStream;
    int choosenAudioStream, choosenVideoStream, choosenSubtitlesStream;
    QString choosenAudioLang, choosenSubtitlesLang;

    double maxThreshold, fps;

    bool quitApp, audioEnabled, videoEnabled, subtitlesEnabled, doSuspend, doRepeat, allowAccurateSeek;
    QTimer timTerminate;
};

#endif // PLAYER_H
