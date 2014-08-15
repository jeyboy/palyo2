#ifndef MASTER_CLOCK_H
#define MASTER_CLOCK_H

//#include <QMutex>
#include "media_player_utils.h"

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

class MasterClock {
//    Q_OBJECT
public:
    ~MasterClock();

    static void create();
    static MasterClock * instance();
    static void close() {
        delete self;
    }

    void reset(uint clock);
    uint computeAudioDelay();
    bool dropVideoPacket();
    uint computeVideoDelay();

    inline double main() { return mainClock; }
    inline void setMain(double newClock) { mainClock = newClock; }
    inline void iterateMain(double offset) { mainClock += offset; }

    inline double mainLastPts() { return mainLastPtsVal; }
    inline void setMainLastPts(double newClock) { mainLastPtsVal = newClock; }

    inline double mainLastDelay() { return mainLastDelayVal; }
    inline void setMainLastDelay(double newClock) { mainLastDelayVal = newClock; }

    inline double audio() { return audioClock; }
    inline void setAudio(double newClock) { audioClock = newClock; }
    inline void iterateAudio(double offset) { audioClock += offset; }

    inline double audioOutput() { return audioOClock; }
    inline void iterateAudioOutput(double offset) { audioOClock += offset; }

    inline double video() { return videoClock; }
    inline void setVideo(double newClock) { videoClock = newClock; }
    inline void iterateVideo(double offset) { videoClock += offset; }

    inline void setVideoNext(double newClock) { videoClockNext = newClock; }

    inline double subtitle() { return subtitlesClock; }
    inline void setSubtitle(double newClock) { subtitlesClock = newClock; }
    inline void iterateSubtitle(double offset) { subtitlesClock += offset; }
private:

    MasterClock() {
        reset(0);
    }

    static MasterClock * self;

    bool half;

    double mainClock;
    double mainLastPtsVal;
    double mainLastDelayVal;

    volatile double audioClock;
    volatile double audioOClock;

    volatile double videoClock;
    volatile double videoClockNext;

    volatile double subtitlesClock;
};

#endif // MASTER_CLOCK_H
