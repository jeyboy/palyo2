#include "gl_output.h"

#include <QDebug>
#include <QTime>

GLOutput::GLOutput(QWidget* parent) : QWidget(parent)
  , frame(new VideoFrame) {

    setAutoFillBackground(true);
    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_UpdatesDisabled);
    setAttribute(Qt::WA_OpaquePaintEvent);

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

void GLOutput::setPauseDelay(int millis) {
    this -> frame -> pts = -millis;
}

void GLOutput::setFrame(VideoFrame * frame) {
    mutex.lock();
    delete this -> frame;
    this -> frame = frame;
    mutex.unlock();
}

void GLOutput::drawNext() {
    if (frame -> close) {
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
//    QGLWidget::paintEvent(event);

    QPainter p(this);
//    p.fillRect(this -> rect(), Qt::black);

    //Set the painter to use a smooth scaling algorithm.
    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
//    p.setRenderHint(QPainter::Antialiasing, 1);

    mutex.lock();
    p.drawPixmap(frame -> calcSize(this -> rect()), frame -> image);
    mutex.unlock();
}
