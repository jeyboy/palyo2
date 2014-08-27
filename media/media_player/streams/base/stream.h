#ifndef STREAM_H
#define STREAM_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/master_clock.h"

#include <QThread>

class Stream : public QThread {
    Q_OBJECT
public:
    Stream(QObject * parent, Priority priority = InheritPriority);
    virtual ~Stream();

    void run();
    void stop();
    void suspend();
    void resume();
protected:
    virtual void routine() = 0;

    volatile bool exitRequired;
    volatile bool pauseRequired;
};

#endif // STREAM_H
