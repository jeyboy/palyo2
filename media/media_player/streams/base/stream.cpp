#include "stream.h"
#include <qDebug>

Stream::Stream(QObject * parent) : QThread(parent)
  , exitRequired(false)
  , pauseRequired(false)
  , eof(false) {   

    connect(parent, SIGNAL(rejectEofRequired()), this, SLOT(rejectEof()));
    connect(parent, SIGNAL(eofDetectedRequired()), this, SLOT(eofDetected()));
    connect(parent, SIGNAL(flushDataRequired()), this, SLOT(flushData()), Qt::BlockingQueuedConnection);
    connect(parent, SIGNAL(suspendRequired()), this, SLOT(suspend()));
    connect(parent, SIGNAL(resumeRequired()), this, SLOT(resume()));
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
