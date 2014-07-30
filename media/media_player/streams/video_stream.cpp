#include "video_stream.h"

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : Stream(context, streamIndex, parent, priority) {

}
