#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent) : QThread(parent)
  , exitRequired(false)
  , pauseRequired(false)
  , eof(false) {

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
}

void Stream::stop() { exitRequired = true; }

void Stream::suspend() { pauseRequired = true; }

void Stream::resume() { pauseRequired = false; }

void Stream::rejectEof() { eof = false; }
void Stream::eofDetected() { eof = true; }
