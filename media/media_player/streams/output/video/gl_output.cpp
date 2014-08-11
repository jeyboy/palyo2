#include "gl_output.h"
#include <QDebug>

#include "gl_output.h"
#include <QDebug>

GLOutput::GLOutput(QWidget* parent) : QGLWidget(parent)
  , preloadedMillis(0)
  , frame(new VideoFrame) {
    show();
}

GLOutput::~GLOutput() {
    delete frame;
    qDeleteAll(videoBuffer);
}

void GLOutput::setFrame(VideoFrame * frame) {
    mutex.lock();
    videoBuffer.append(frame);
    preloadedMillis += frame -> interval;
    mutex.unlock();
}

double GLOutput::millisPreloaded() {
    return preloadedMillis;
}

void GLOutput::paintEvent(QPaintEvent*) {
    if (!videoBuffer.isEmpty()) {
        QPainter p(this);

        //Set the painter to use a smooth scaling algorithm.
        p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
        delete frame;
        mutex.lock();
        frame = videoBuffer.takeFirst();
        mutex.unlock();
        p.drawImage(this -> rect(), *frame -> image);
        preloadedMillis -= frame -> interval;
    }

    if (!timer.isActive())
        timer.singleShot(frame -> interval * 1000, this, SLOT(repaint()));
}
