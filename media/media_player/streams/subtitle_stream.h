#ifndef SUBTITLE_STREAM_H
#define SUBTITLE_STREAM_H

#include "stream.h"

class SubtitleStream : public Stream {
public:
    SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
};

#endif // SUBTITLE_STREAM_H
