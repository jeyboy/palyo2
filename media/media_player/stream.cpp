#include "stream.h"

Stream::Stream(AVFormatContext * context, uint streamIndex) : QObject(), state(true), stream(0), codec(0) {
    uindex = streamIndex;

    AVStream * stream = context -> streams[uindex];

    if (stream == 0) {
        state = false;
        return;
    }

    AVCodecContext * codec_context = stream -> codec;
    codec = avcodec_find_decoder(codec_context -> codec_id);

    if (codec == 0) {
        state = false;
        return;
    }

    if (avcodec_open2(codec_context, codec, NULL) < 0) {
        state = false;
    }
}

Stream::~Stream() {
    delete stream;

    if (codec) {
        avcodec_close(codec);
        delete codec;
    }
}
