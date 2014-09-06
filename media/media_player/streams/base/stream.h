#ifndef STREAM_H
#define STREAM_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/master_clock.h"

#include <QThread>

class Stream : public QThread {
    Q_OBJECT
public:
    Stream(QObject * parent);
    virtual ~Stream();

    void run();
    void stop();

public slots:
    void rejectEof();
    void eofDetected();

    virtual void suspend();
    virtual void resume();
    virtual void flushData() {}

protected:
    virtual void routine() = 0;

    bool exitRequired;
    bool pauseRequired;
    bool eof;
};

#endif // STREAM_H
