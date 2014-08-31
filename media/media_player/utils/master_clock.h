#ifndef MASTER_CLOCK_H
#define MASTER_CLOCK_H

//#include <QMutex>
#include "media_player_utils.h"

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

class MasterClock : public QObject {
    Q_OBJECT
public:
    ~MasterClock();

    static void create();
    static MasterClock * instance();
    static void close() {
        delete self;
    }

    void reset(uint clock);
    int computeAudioDelay();
    int computeVideoDelay(double compClock, double compClockNext);

    inline bool demuxeRequired() { return fabs(audioClock - videoClock) > 0.1; } //1 sec

    inline double main_clock() { return mainClock; }
    inline void setMain(double newClock) { mainClock = newClock; }
    inline void iterateMain(double offset) { mainClock += offset; }

    inline double mainLastPts() { return mainLastPtsVal; }
    inline void setMainLastPts(double newClock) { mainLastPtsVal = newClock; }

    inline double mainLastDelay() { return mainLastDelayVal; }
    inline void setMainLastDelay(double newClock) { mainLastDelayVal = newClock; }

    inline double audio() { return audioClock; }
    inline void setAudio(double newClock) {
        audioOClock = av_gettime() + (newClock - audioClock) * 1000000;
        audioClock = newClock;
        emit positionUpdated(audioClock);
        emit __positionUpdated(audioClock * 1000);
    }
    inline void iterateAudio(double offset) {
        audioOClock = av_gettime() + (offset * 1000000);
        audioClock += offset;
        emit positionUpdated(audioClock);
        emit __positionUpdated(audioClock * 1000);
    }

    inline double video() { return videoClock; }
    inline void setVideo(double newClock) { videoClock = newClock; }
    inline void iterateVideo(double offset) { videoClock += offset; }

    inline void setVideoSync(double newClock) { videoSyncClock = newClock; }
    inline void setVideoNext(double newClock) { videoClockNext = newClock; }

    inline double subtitle() { return subtitlesClock; }
    inline void setSubtitle(double newClock) { subtitlesClock = newClock; }
    inline void iterateSubtitle(double offset) { subtitlesClock += offset; }
signals:
    void positionUpdated(double);
    void __positionUpdated(int);

private:

    MasterClock();

    static MasterClock * self;

    double mainClock;
    double mainLastPtsVal;
    double mainLastDelayVal;
    double prevDelay;

    volatile double audioClock;
    int64_t audioOClock;

    volatile double videoClock;
    volatile double videoClockNext;
    volatile double videoSyncClock;

    volatile double subtitlesClock;
};

#endif // MASTER_CLOCK_H
