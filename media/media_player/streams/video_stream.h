#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "stream.h"

class VideoStream : public Stream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream() {}
};

#endif // VIDEO_STREAM_H
