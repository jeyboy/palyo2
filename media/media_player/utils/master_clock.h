#ifndef MASTER_CLOCK_H
#define MASTER_CLOCK_H

#include <QMutex>

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
    double computeDelay();

    inline double main() { return mainClock; }
    inline double setMain(double newClock) { mainClock = newClock; }
    inline double iterateMain(double offset) { mainClock += offset; }

    inline double mainLastPts() { return mainLastPtsVal; }
    inline double setMainLastPts(double newClock) { mainLastPtsVal = newClock; }

    inline double mainLastDelay() { return mainLastDelayVal; }
    inline double setMainLastDelay(double newClock) { mainLastDelayVal = newClock; }

    inline double audio() { return audioClock; }
    inline double setAudio(double newClock) { audioClock = newClock; }
    inline double iterateAudio(double offset) { audioClock += offset; }

    inline double audioOutput() { return audioOClock; }
    inline double iterateAudioOutput(double offset) { audioOClock += offset; }

    inline double video() { return videoClock; }
    inline double setVideo(double newClock) { videoClock = newClock; }
    inline double iterateVideo(double offset) { videoClock += offset; }

    inline double setVideoNext(double newClock) { videoClockNext = newClock; }

    inline double subtitle() { return subtitlesClock; }
    inline double setSubtitle(double newClock) { subtitlesClock = newClock; }
    inline double iterateSubtitle(double offset) { subtitlesClock += offset; }
private:

    MasterClock() {
        reset(0);
    }

    static MasterClock * self;

    double mainClock;
    double mainLastPtsVal;
    double mainLastDelayVal;

    volatile double audioClock;
    volatile double audioOClock;

    volatile double videoClock;
    volatile double videoClockNext;

    volatile double subtitlesClock;

//    uint audioOClock;
//    uint videoOClock;
//    uint subtitlesOClock;

    QMutex * audioMutex;
    QMutex * videoMutex;
    QMutex * subtitlesMutex;

//    QMutex * audioOMutex;
//    QMutex * videoOMutex;
//    QMutex * subtitlesOMutex;


};

#endif // MASTER_CLOCK_H
