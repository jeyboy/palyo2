#include "stream.h"
#include <qDebug>

Stream::Stream(AVFormatContext * context, uint streamIndex, Priority priority) : QThread(), state(true), exitRequired(false), stream(0), codec(0) {
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

    if (state)
        start(priority);
}

Stream::~Stream() {
    if (codec) {
        avcodec_close(stream -> codec);
        delete codec;
    }

    delete stream;
}

void Stream::run() {
    while(!exitRequired) {

    }

    qDebug() << "Stopped";
}

void Stream::stop() {
    exitRequired = true;
}
