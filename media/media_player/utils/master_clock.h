#ifndef MASTER_CLOCK_H
#define MASTER_CLOCK_H

#include <QMutex>

class MasterClock {
//    Q_OBJECT
public:
//    void updateSpeed(float scale);

    inline uint main() { return mainClock; }
    inline uint setMain(uint newClock) { mainClock = newClock; }
    inline uint iterateMain(uint offset) { mainClock += offset; }

private:
    uint mainClock;
    volatile uint audioClock;
    volatile uint videoClock;
    volatile uint subtitlesClock;

    uint audioOClock;
    uint videoOClock;
    uint subtitlesOClock;

    QMutex * audioMutex;
    QMutex * videoMutex;
    QMutex * subtitlesMutex;

    QMutex * audioOMutex;
    QMutex * videoOMutex;
    QMutex * subtitlesOMutex;


};

#endif // MASTER_CLOCK_H
