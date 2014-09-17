#include "base_output.h"

BaseOutput::BaseOutput(QObject * parent, int width, int height) : Stream(parent) {
    output = new VideoOutput(this, width, height);
    connect(this, SIGNAL(framePrepared(void *)), output, SLOT(setFrame(void *)), Qt::DirectConnection);
    start();
}

BaseOutput::~BaseOutput() {
    flushData();
}

void BaseOutput::flushData() {
    qDeleteAll(frames);
    frames.clear();
}

void BaseOutput::proceedFrame(VideoFrame * frame) {
    frames.append(frame);
}

QWidget * BaseOutput::widget() const {
    return output;
}

void BaseOutput::routine() {
    if (frames.isEmpty()) {
        msleep(5);
        return;
    }

    VideoFrame * frame = frames.takeFirst();
    msleep(frame -> calcDelay());
    emit framePrepared(frame -> image);
    delete frame;
}
