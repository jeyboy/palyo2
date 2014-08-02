#include "subtitle_stream.h"

SubtitleStream::SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority) {

}

void SubtitleStream::suspendOutput() {

}
void SubtitleStream::resumeOutput() {

}

void SubtitleStream::routine() {
//    mutex -> lock();
//    AVPacket * packet = packets.takeFirst();
//    mutex -> unlock();

}

