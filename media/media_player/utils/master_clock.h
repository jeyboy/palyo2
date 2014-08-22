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

    void reset(uint clock, bool has_video);
    int computeVideoDelay(double compClock, double compClockNext);

    inline bool demuxeRequired() { return fabs(audioClock - videoClock) > 0.1; } //1 sec

    inline double main_clock() { return mainClock; }

    inline double audio() { return audioClock; }
    inline void setAudio(double newClock) {
        audioClock = newClock;
        setMain(newClock);
    }
    inline void iterateAudio(double offset) {
        audioClock += offset;
        setMain(audioClock);
    }

    inline double video() { return videoClock; }
    inline void setVideo(double newClock) {
        videoClock = newClock;
        if (hasVideo)
            setMain(newClock);
    }
    inline void iterateVideo(double offset) {
        videoClock += offset;
        if (hasVideo)
            setMain(videoClock);
    }

    inline double subtitle() { return subtitlesClock; }
    inline void setSubtitle(double newClock) { subtitlesClock = newClock; }
    inline void iterateSubtitle(double offset) { subtitlesClock += offset; }
signals:
    void positionUpdated(double);
    void __positionUpdated(int);

private:
    void setMain(double newClock) {
        mainClock = newClock;
        emit positionUpdated(mainClock);
        emit __positionUpdated(mainClock * 1000);
    }

    MasterClock();

    static MasterClock * self;

    bool hasVideo;

    double prevDelay;

    double mainClock;

    double audioClock;

    double videoClock;
    double videoClockNext;

    double subtitlesClock;
};

#endif // MASTER_CLOCK_H
