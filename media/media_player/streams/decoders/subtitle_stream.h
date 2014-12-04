#ifndef SUBTITLE_STREAM_H
#define SUBTITLE_STREAM_H

#include "media/media_player/streams/base/media_stream.h"

class SubtitleStream : public MediaStream {
public:
    SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);

    int calcDelay();

    inline bool isBlocked() { return false; }
    void suspend();
    void resume();
    void flushData();
protected:
    void routine();
};

#endif // SUBTITLE_STREAM_H
