#ifndef SUBTITLE_STREAM_H
#define SUBTITLE_STREAM_H

#include "stream.h"

class SubtitleStream : public Stream {
public:
    SubtitleStream(AVFormatContext * context, uint streamIndex, Priority priority = InheritPriority);
};

#endif // SUBTITLE_STREAM_H
