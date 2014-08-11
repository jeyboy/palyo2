#include "gl_output.h"
#include <QDebug>

GLOutput::GLOutput(QWidget* parent) : QGLWidget(parent)
 , img(0)
 , interval(1000/25) {
    show();
}

GLOutput::~GLOutput() {
    qDeleteAll(videoBuffer);
}

void GLOutput::setImage(QImage * image) {
    mutex.lock();
    videoBuffer.append(image);
    mutex.unlock();
}

void GLOutput::paintEvent(QPaintEvent*) {
    qDebug() << "DRAW";

    if (!videoBuffer.isEmpty()) {
        QPainter p(this);

        //Set the painter to use a smooth scaling algorithm.
        p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
        delete img;
        mutex.lock();
        img = videoBuffer.takeFirst();
        mutex.unlock();
        p.drawImage(this -> rect(), *img);
    }

    timer.singleShot(interval, this, SLOT(repaint()));
}
