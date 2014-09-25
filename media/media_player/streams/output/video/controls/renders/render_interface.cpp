#include "render_interface.h"

#include <QDebug>

RenderInterface::RenderInterface(QWidget* parent) : QWidget(parent)
  , vFrame(0) {

    init = false;

    setAttribute(Qt::WA_OpaquePaintEvent,true);
    setAttribute(Qt::WA_PaintOnScreen,true);
}

RenderInterface::~RenderInterface() {
    mutex.lock();
    delete vFrame;
    mutex.unlock();
}

void RenderInterface::setFrame(VideoFrame * frame) {
    if (frame) {
        mutex.lock();
        delete vFrame;
        vFrame = frame;
        mutex.unlock();
        emit updated();
        repaint();
    }
}

//void RenderInterface::repaint() {
//    QWidget::repaint();
//    paintFrame();
//}

void RenderInterface::closeEvent(QCloseEvent *) {
    emit closed();
}

void RenderInterface::resizeEvent(QResizeEvent * event) {
    QWidget::resizeEvent(event);
    resizeViewport(width(), height());
    if (vFrame)
        output_rect = vFrame -> calcSize(this -> rect());
}
