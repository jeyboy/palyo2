#include "media_stream.h"

MediaStream::MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority)
    : IMediaStream(context, streamIndex), Stream(parent)
  , waitMillis(2)
  , eof(false) {

    if (valid) {
        connect(parent, SIGNAL(eofRejectRequired()), this, SLOT(rejectEof()));
        connect(parent, SIGNAL(eofDetectRequired()), this, SLOT(eofDetected()));
        connect(parent, SIGNAL(flushDataRequired()), this, SLOT(flushData()), Qt::DirectConnection);
        connect(parent, SIGNAL(suspendRequired()), this, SLOT(suspendStream()));
        connect(parent, SIGNAL(resumeRequired()), this, SLOT(resumeStream()));

        start(priority);
    }
}

MediaStream::~MediaStream() {

}

void MediaStream::rejectEof() { eof = false; }
void MediaStream::eofDetected() { eof = true; }
void MediaStream::suspendStream() { Stream::suspend(); }
void MediaStream::resumeStream() { Stream::resume(); }
