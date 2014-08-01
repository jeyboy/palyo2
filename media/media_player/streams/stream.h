#ifndef STREAM_H
#define STREAM_H

#include "../media_player_utils.h"
#include <QThread>
#include <qDebug>

class Stream : public QThread {
    Q_OBJECT
public:
    Stream(QObject * parent, Priority priority = InheritPriority);
    virtual ~Stream();

    inline bool isValid() const { return state; }
    inline uint index() const { return uindex; }

    void run();
    void stop();
protected:
    virtual void routine() = 0;

    bool state;
    volatile bool exitRequired;
    AVStream * stream;
    uint uindex;
    AVCodec * codec; // this var is possible to get through stream -> codec()
    AVFrame * frame;
};

#endif // STREAM_H
