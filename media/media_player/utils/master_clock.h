#ifndef MASTER_CLOCK_H
#define MASTER_CLOCK_H

//#include <QMutex>
#include "media/media_player/utils/media_player_utils.h"

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

class MasterClock/* : public QObject*/ {
//    Q_OBJECT
public:
    MasterClock(void * parent);

    ~MasterClock();

    void reset();
    void resetMain();

//    int computeAudioDelay();

//    bool skipAudioRequired();
//    bool skipVideoRequired();
    int computeVideoDelay(double compClock, double compClockNext);

    inline void * mediaPlayer() const { return player; }

    // sync by audio clock
    inline double position() const { return audioClock; }

    double main_clock();
    void setMain(double newClock);
    void iterateMain(double offset);

    double audio();
    void setAudio(double newClock);
    void iterateAudio(double offset);

    double video();
    void setVideo(double newClock);
    void iterateVideo(double offset);

    void setVideoNext(double newClock);

    double subtitle();
    void setSubtitle(double newClock);
    void iterateSubtitle(double offset);
private:
    void * player;

    double mainClock;
    double mainLastPtsVal;
    double mainLastDelayVal;
    double prevDelay;

    volatile double audioClock;
    int64_t audioOClock;

    volatile double videoClock;
    volatile double videoClockNext;
//    volatile double videoSyncClock;

    volatile double subtitlesClock;
};

#endif // MASTER_CLOCK_H
