#ifndef SUBTITLE_STREAM_H
#define SUBTITLE_STREAM_H

#include "media_stream.h"

class SubtitleStream : public MediaStream {
public:
    SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);

    void suspendOutput();
    void resumeOutput();
protected:
    void routine();
};

#endif // SUBTITLE_STREAM_H
