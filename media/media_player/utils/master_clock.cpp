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

    audioOClock = 0;
    videoClockNext = 0;
    mainLastPtsVal = 0;
    mainLastDelayVal = 0.20;
}

uint MasterClock::computeAudioDelay() {
    double diff = videoClockNext - audioOClock;
    if (diff < 0) {
        qDebug() << "!!!!!!!!!!!!!!! " << diff;
        return fabs(diff) * 200;
    }

    return 0;
}

uint MasterClock::computeVideoDelay() {
    qDebug() << "-----------------------------------------";
    double delay = videoClock - mainLastPtsVal;
    if (delay <= 0.0 || delay >= 1.0) {
            // Delay incorrect - use previous one
            delay = mainLastDelayVal;
    }
    // Save for next time
    mainLastPtsVal = videoClockNext;
    mainLastDelayVal = delay;

    double diff = videoClockNext - audioOClock;
    qDebug() << "diff " << diff << " " << videoClockNext << " " << audioOClock;
    double sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
    if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
            if (diff <= -sync_threshold) {
                    delay = 0;
            } else if (diff >= sync_threshold) {
                    delay = diff; //4 * delay;
            }
    }
    qDebug() << "total delay " << delay;

    mainClock += delay;

    double actual_delay = (mainClock - (av_gettime() / 1000000.0));
    if(actual_delay < 0.010) {
            /* Really it should skip the picture instead */
            actual_delay = 0.010;
    }
    return actual_delay * 100;
}
