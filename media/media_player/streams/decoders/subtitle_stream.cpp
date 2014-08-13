#include "subtitle_stream.h"

SubtitleStream::SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority) {

}

void SubtitleStream::suspendOutput() {

}
void SubtitleStream::resumeOutput() {

}

void SubtitleStream::routine() {
    mutex -> lock();
    if (packets.isEmpty()) {
        mutex -> unlock();
        pauseRequired = finishAndPause;
        if (finishAndExit) stop();
        msleep(waitMillis);
        return;
    }

    AVPacket * packet = packets.takeFirst();
    mutex -> unlock();


}
