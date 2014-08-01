#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent, Priority priority) : QThread(parent), exitRequired(false) {
    start(priority);
}

Stream::~Stream() {
    qDebug() << " ******* " << state;
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
        routine();
    }

    qDebug() << "Stopped";
}

void Stream::stop() {
    exitRequired = true;
}
