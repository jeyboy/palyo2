#include "subtitle_stream.h"

SubtitleStream::SubtitleStream(AVFormatContext * context, uint streamIndex, Priority priority)
    : Stream(context, streamIndex, priority) {

}

