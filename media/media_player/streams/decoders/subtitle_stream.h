#ifndef SUBTITLE_STREAM_H
#define SUBTITLE_STREAM_H

#include "media/media_player/streams/base/media_stream.h"

class SubtitleStream : public MediaStream {
public:
    SubtitleStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority = InheritPriority);
    ~SubtitleStream();

//    int calcDelay();

    inline bool isBlocked() { return false; }
    void suspend();
    void resume();
    void flushData();

    void nextFrame(void *& ret) { ret = 0; } // stub
protected:
    void routine();
};

#endif // SUBTITLE_STREAM_H
