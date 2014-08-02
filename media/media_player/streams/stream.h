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
protected:
    virtual void routine() = 0;

    volatile bool exitRequired;
    QMutex * mutex;
};

#endif // STREAM_H
