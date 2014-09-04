#include "media_stream.h"

MediaStream::MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority)
    : IMediaStream(context, streamIndex), Stream(parent, priority)
  , waitMillis(2)
  , eof(false) {

    if (valid)
        start(priority);
}

MediaStream::~MediaStream() {
    qDebug() << " ******* " << valid;

    avcodec_close(codec_context);

    if (frame)
        av_frame_free(&frame);

    foreach(AVPacket * pack, packets)
        av_free_packet(pack);
}

void MediaStream::resume() {
    Stream::resume();
}
