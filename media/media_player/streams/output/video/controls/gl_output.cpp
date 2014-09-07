#include "gl_output.h"

#include <QDebug>

GLOutput::GLOutput(QWidget* parent) : QGLWidget(parent)
  , frame(new VideoFrame) {

//    setAutoBufferSwap(false);
//    setAutoFillBackground(false);

    drawNext();
}

GLOutput::~GLOutput() {
    mutex.lock();
    delete frame;
    mutex.unlock();
}

void GLOutput::setPauseDelay() {
    this -> frame -> pts = -1;
}

void GLOutput::setFrame(VideoFrame * frame) {
    mutex.lock();
    delete this -> frame;
    this -> frame = frame;
    mutex.unlock();
}

void GLOutput::drawNext() {
    if (frame -> image == 0) {
        close();
        return;
    }

    emit updated();
    repaint();

    timer.singleShot(frame -> calcDelay(), this, SLOT(drawNext()));
}

void GLOutput::closeEvent(QCloseEvent *) {
    emit closed();
}

void GLOutput::paintEvent(QPaintEvent *) {
    QPainter p(this);

    //Set the painter to use a smooth scaling algorithm.
    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
    mutex.lock();
    p.drawImage(this -> rect(), *frame -> image);
    mutex.unlock();

    //    QGLWidget::paintEvent(event);
}
