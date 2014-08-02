#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent, Priority priority) : QThread(parent)
  , exitRequired(false)
  , pauseRequired(false) {

    mutex = new QMutex();
//    setPriority(priority);
}

Stream::~Stream() {
    qDebug() << "STream";
    stop();

    delete mutex;
}

void Stream::run() {
    while(!exitRequired) {
        if (!pauseRequired)
            routine();
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
