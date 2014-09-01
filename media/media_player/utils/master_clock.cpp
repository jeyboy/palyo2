#include "master_clock.h"
#include "math.h"

MasterClock *MasterClock::self = 0;

void MasterClock::create() {
    if(!self)
        self = new MasterClock();
}

MasterClock *MasterClock::instance() {
    return self;
}

MasterClock::MasterClock() : QObject() {
    reset(0);
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

int MasterClock::computeAudioDelay() {
    int res = (audioOClock - av_gettime());
    res -= 500;
    if (res < 0 || res > 100000) res = 0;
    return res;
}

int MasterClock::computeVideoDelay(double compClock, double compClockNext) {
    setVideo(compClock);
    videoClockNext = compClockNext;

//    qDebug() << "-----------------------------------------";
    double delay = videoClock - mainLastPtsVal;
    if (delay <= 0.0 || delay >= 1.0) {
        delay = mainLastDelayVal;
    }
    // Save for next time
    mainLastPtsVal = videoClockNext;
    mainLastDelayVal = delay;

    double diff = videoClockNext - audioClock;
//    qDebug() << "v - a  " << diff << " " << videoClockNext << " " << audioClock;
    double sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
    if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
            if (diff <= -sync_threshold) {
                    delay = 0;
            } else if (diff >= sync_threshold) {
                delay = diff;
//                    delay = diff + temp * 1.5; //4 * delay;
            }
    }


//    qDebug() << "total delay " << delay;

    mainClock += delay;

    //    av_gettime() / 1000000.0) is a internal clock
    double actual_delay = (mainClock - (av_gettime() / 1000000.0));
    if (actual_delay < 0.010) {
        return 0;
//            /* Really it should skip the picture instead */
//            actual_delay = 0.010;
    }

    return actual_delay * 999; // 100
}
