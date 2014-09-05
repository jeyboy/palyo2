#include "media_stream.h"

MediaStream::MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority)
    : IMediaStream(context, streamIndex), Stream(parent)
  , waitMillis(2) {

    if (valid) start(priority);
}

MediaStream::~MediaStream() {
}
