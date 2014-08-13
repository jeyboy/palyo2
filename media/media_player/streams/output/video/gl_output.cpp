#include "gl_output.h"
#include <QDebug>

#include "gl_output.h"
#include <QDebug>

GLOutput::GLOutput(QWidget* parent) : QGLWidget(parent)
  , preloadedMillis(0)
  , frame(new VideoFrame) {
    show();
    drawNext();
}

GLOutput::~GLOutput() {
    delete frame;
    qDeleteAll(videoBuffer);
}

void GLOutput::setFrame(VideoFrame * frame) {
    mutex.lock();
    videoBuffer.append(frame);
    preloadedMillis += frame -> interval;
    qDebug() << frame -> interval << " " << preloadedMillis;
    mutex.unlock();
}

double GLOutput::millisPreloaded() {
    return preloadedMillis;
}

void GLOutput::drawNext() {
    if (!videoBuffer.isEmpty()) {
        mutex.lock();
        delete frame;
        frame = videoBuffer.takeFirst();
        mutex.unlock();
        preloadedMillis -= frame -> interval;
    }

    timer.singleShot(frame -> interval * 100, (GLOutput *)this, SLOT(drawNext()));
    repaint();
}

void GLOutput::paintEvent(QPaintEvent*) {
    QPainter p(this);

    //Set the painter to use a smooth scaling algorithm.
    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
    mutex.lock();
    p.drawImage(this -> rect(), *frame -> image);
    mutex.unlock();
}
