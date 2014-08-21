#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent, Priority priority) : QThread(parent)
  , exitRequired(false)
  , pauseRequired(false) {

//    setPriority(priority);
}

Stream::~Stream() {
    qDebug() << "STream";
    stop();
}

void Stream::run() {
    while(!exitRequired) {
        if (!pauseRequired)
            routine();
        else sleep(1);
    }

    qDebug() << "Stopped";
}

void Stream::stop() {
    exitRequired = true;
}

void Stream::suspend() {
    pauseRequired = true;
}
void Stream::resume() {
    pauseRequired = false;
}