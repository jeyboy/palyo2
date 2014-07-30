#include "video_stream.h"

VideoStream::VideoStream(AVFormatContext * context, uint streamIndex, Priority priority)
    : Stream(context, streamIndex, priority) {

}
