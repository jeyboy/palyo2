#include "master_clock.h"
#include "math.h"

MasterClock *MasterClock::self = 0;

void MasterClock::create() {
    if(!self)
        self = new MasterClock();
}

MasterClock *MasterClock::instance() {
//    if(!self)
//        self = new MasterClock();
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
    mainLastDelayVal = 0.20;
}

double MasterClock::computeDelay() {
    qDebug() << "-----------------------------------------";
    double delay = videoClock - mainLastPtsVal;
    qDebug() << "delay " << delay << " last delay " << mainLastDelayVal;
    if (delay <= 0.0 || delay >= 1.0) {
            // Delay incorrect - use previous one
            delay = mainLastDelayVal;
    }
    // Save for next time
    mainLastPtsVal = videoClockNext;
    mainLastDelayVal = delay;
    qDebug() << "next video " << videoClockNext;
    qDebug() << "audio output " << audioOClock;

    double diff = videoClockNext - audioOClock;
    qDebug() << "diff " << diff;
    double sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
    if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
            if (diff <= -sync_threshold) {
                    delay = 0;
            } else if (diff >= sync_threshold) {
                    delay = 4 * delay;
            }
    }
    qDebug() << "total delay " << delay;

    mainClock += delay;
    qDebug() << "main " << mainClock;

    double actual_delay = (mainClock - (av_gettime() / 1000000.0));// + (audioOClock - videoClock);
    qDebug() << "actual " << actual_delay;
    if(actual_delay < 0.010) {
            /* Really it should skip the picture instead */
            actual_delay = 0.010;
    }
    qDebug() << "actual2 " << actual_delay;
    return actual_delay;
}
