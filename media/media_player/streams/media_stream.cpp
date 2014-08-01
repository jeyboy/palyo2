#include "media_stream.h"

MediaStream::MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority) : Stream(parent, priority), state(true), stream(0), codec(0) {
    if (streamIndex < 0 || streamIndex == AVERROR_STREAM_NOT_FOUND || streamIndex == AVERROR_DECODER_NOT_FOUND) {
        state = false;
        stop();
        return;
    }

    uindex = streamIndex;

    stream = context -> streams[uindex];

    if (stream == 0) {
        state = false;
        stop();
        return;
    }

    AVCodecContext * codec_context = stream -> codec;
    codec = avcodec_find_decoder(codec_context -> codec_id);

    //wtf
    if (codec -> capabilities & CODEC_CAP_TRUNCATED)
        codec_context -> flags |= CODEC_FLAG_TRUNCATED;
    //////

    if (codec == 0) {
        state = false;
        stop();
        return;
    }

    if (avcodec_open2(codec_context, codec, NULL) < 0) {
        state = false;
        stop();
    }
}

MediaStream::~MediaStream() {

}
