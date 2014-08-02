#ifndef STREAM_H
#define STREAM_H

#include "../media_player_utils.h"
#include <QThread>
#include <QMutex>
#include <qDebug>

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
    bool pauseRequired;
    QMutex * mutex;
};

#endif // STREAM_H
