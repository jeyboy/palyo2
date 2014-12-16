#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent) : QThread(parent)
  , exitRequired(false)
  , pauseRequired(false)
  , skip(false)
  , semaphore(0) {
}

Stream::~Stream() {
    qDebug() << "STream";
    stop();
}

void Stream::run() {
    while(!exitRequired) {
        if (!pauseRequired)
            routine();
        else
            if (!skip)
                msleep(50);
    }
}

void Stream::stop() { exitRequired = true; }

void Stream::suspend() { pauseRequired = true; }

void Stream::resume() { pauseRequired = false; }

void Stream::setSemaphore(QSemaphore * sema) {
    semaphore = sema;
}
