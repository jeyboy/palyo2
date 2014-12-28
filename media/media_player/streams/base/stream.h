#ifndef STREAM_H
#define STREAM_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/master_clock.h"

#include <QThread>
#include <QSemaphore>

class Stream : public QThread {
public:
    Stream(QObject * parent);
    virtual ~Stream();

    void run();
    void stop();

    void suspend();
    void resume();

    void setSemaphore(QSemaphore *);

    inline bool isSuspended() const { return pauseRequired; }

protected:
    virtual void routine() = 0;

    bool exitRequired;
    bool pauseRequired;
    bool skip;

    QSemaphore * semaphore;
};

#endif // STREAM_H
