#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent, Priority priority) : QThread(parent)
  , exitRequired(false)
  , pauseRequired(false)
  , mutex(0) {
    setPriority(priority);
}

Stream::~Stream() {
    qDebug() << "STream";
    stop();

// this strings throw error
//    if (stream) {
//        avcodec_close(stream -> codec);
//    }

//    delete codec;
//    delete stream;
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
