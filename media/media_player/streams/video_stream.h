#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "stream.h"

class VideoStream : public Stream {
public:
    VideoStream(AVFormatContext * context, uint streamIndex, Priority priority = InheritPriority);
};

#endif // VIDEO_STREAM_H
