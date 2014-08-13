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
    mutex.lock();
    qDeleteAll(videoBuffer);
    delete frame;
    mutex.unlock();
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

void GLOutput::drawNext() {
    if (!videoBuffer.isEmpty()) {
        mutex.lock();
        delete frame;
        frame = videoBuffer.takeFirst();
        mutex.unlock();
        preloadedMillis -= frame -> interval;
        setWindowTitle(QString::number(videoBuffer.size()));
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
