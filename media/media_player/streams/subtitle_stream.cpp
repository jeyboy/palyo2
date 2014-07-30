#include "subtitle_stream.h"

SubtitleStream::SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : Stream(context, streamIndex, parent, priority) {

}

