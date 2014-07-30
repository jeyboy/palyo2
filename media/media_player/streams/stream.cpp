#include "stream.h"
#include <qDebug>

Stream::Stream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority) : QThread(parent), state(true), exitRequired(false), stream(0), codec(0) {
    if (streamIndex < 0 || streamIndex == AVERROR_STREAM_NOT_FOUND || streamIndex == AVERROR_DECODER_NOT_FOUND) {
        state = false;
        return;
    }

    uindex = streamIndex;

    stream = context -> streams[uindex];

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
    qDebug() << " ******* " << state;
    stop();

//    if (stream) {
//        avcodec_close(stream -> codec);
//    }

//    delete codec;
//    delete stream;
}

void Stream::run() {
    while(!exitRequired) {

    }

    qDebug() << "Stopped";
}

void Stream::stop() {
    exitRequired = true;
}
