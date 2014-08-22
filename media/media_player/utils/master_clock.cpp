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
    reset(0, false);
}

MasterClock::~MasterClock() {

}

void MasterClock::reset(uint clock, bool has_video) {
    setAudio(clock);
    setVideo(clock);
    setSubtitle(clock);

    videoClockNext = 0;
    hasVideo = has_video;
}

int MasterClock::computeVideoDelay(double compClock, double compClockNext) {
    setVideo(compClock);
    videoClockNext = compClockNext;

    double diff = videoClockNext - audioClock;
    qDebug() << "v - a  " << diff << " " << videoClockNext << " " << audioClock;
    double sync_threshold = FFMAX(AV_SYNC_THRESHOLD, diff);
    if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
            if (diff <= -sync_threshold) {
                    diff = 0;
            } else if (diff >= sync_threshold) {
                double temp = diff - (diff - prevDelay);
                if (temp > 0)
                    diff += temp * 1.2;
//                    delay = diff + temp * 1.5; //4 * delay;
            }
    }

    prevDelay = diff;
    return diff * 100;
}
