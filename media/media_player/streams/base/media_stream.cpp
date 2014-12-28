#include "media_stream.h"

MediaStream::MediaStream(AVFormatContext * context, MasterClock * clock, int streamIndex, QObject * parent, Priority priority)
    : IMediaStream(context, streamIndex)
    , Stream(parent)
    , clock(clock)
    , eof(false) {

    if (valid) {
        connect(parent, SIGNAL(eofRejectRequired()), this, SLOT(rejectEof()), Qt::DirectConnection);
        connect(parent, SIGNAL(eofDetectRequired()), this, SLOT(eofDetected()), Qt::DirectConnection);
        connect(parent, SIGNAL(flushDataRequired()), this, SLOT(flushData()), Qt::DirectConnection);
        connect(parent, SIGNAL(suspendRequired()), this, SLOT(suspendStream()), Qt::DirectConnection);
        connect(parent, SIGNAL(resumeRequired()), this, SLOT(resumeStream()), Qt::DirectConnection);

        start(priority);
    }
}

MediaStream::~MediaStream() {

}

void MediaStream::suspend() {
    Stream::suspend();
    emit suspended();
}

void MediaStream::rejectEof() { eof = false; }
void MediaStream::eofDetected() { eof = true; }
void MediaStream::suspendStream() { Stream::suspend(); }
void MediaStream::resumeStream() { Stream::resume(); }
