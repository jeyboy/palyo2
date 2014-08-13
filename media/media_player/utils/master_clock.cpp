#include "master_clock.h"
#include "math.h"

MasterClock *MasterClock::self = 0;

void MasterClock::create() {
    if(!self)
        self = new Extensions();
}

MasterClock *MasterClock::instance() {
//    if(!self)
//        self = new Extensions();
    return self;
}

MasterClock::~MasterClock() {

}

void MasterClock::reset(uint clock) {
    setMain(clock);
    setAudio(clock);
    setVideo(clock);
    setSubtitle(clock);

    videoClockNext = 0;
    mainLastPtsVal = 0;
    mainLastDelayVal = 0.40;
}

double MasterClock::computeDelay() {
    double delay = videoClock - mainLastPtsVal;
    if (delay <= 0.0 || delay >= 1.0) {
            // Delay incorrect - use previous one
            delay = mainLastDelayVal;
    }
    // Save for next time
    mainLastPtsVal = videoClockNext;
    mainLastDelayVal = delay;

    double diff = videoClockNext - audioOClock;
    double sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
    if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
            if (diff <= -sync_threshold) {
                    delay = 0;
            } else if (diff >= sync_threshold) {
                    delay = 2 * delay;
            }
    }

    mainClock += delay;

    double actual_delay = mainClock - (av_gettime() / 1000000.0);
    if(actual_delay < 0.010) {
            /* Really it should skip the picture instead */
            actual_delay = 0.010;
    }
    return actual_delay;
}