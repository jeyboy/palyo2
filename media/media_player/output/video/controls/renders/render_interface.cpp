#include "render_interface.h"

#include <QDebug>

RenderInterface::RenderInterface(QWidget* parent) : QWidget(parent)
  , fpsCounter(0)
  , vFrame(0) {

    init = false;

    setFocusPolicy(Qt::StrongFocus);

    setAttribute(Qt::WA_OpaquePaintEvent, true);
    frameInit();
    fpsCalculation();
}

RenderInterface::~RenderInterface() {
    mutex.lock();
    delete vFrame;
    mutex.unlock();
}

void RenderInterface::redrawed() { fpsCounter++; }

void RenderInterface::frameInit() {
    VideoFrame * frame = 0;
    emit frameNeeded((void *&)frame);
    if (frame) {
        if (!frame -> skip()) {
            mutex.lock();
            delete vFrame;
            vFrame = frame;
            mutex.unlock();
            emit updated();
            repaint();
        }
        frameTimer.singleShot(frame -> calcDelay(), this, SLOT(frameInit()));
    } else frameTimer.singleShot(5, this, SLOT(frameInit()));
}

void RenderInterface::fpsCalculation() {
    emit fpsChanged(fpsCounter);

    fpsCounter = 0;
    timer.singleShot(1000, this, SLOT(fpsCalculation()));
}

void RenderInterface::closeEvent(QCloseEvent *) {
    emit closed();
}

void RenderInterface::resizeEvent(QResizeEvent * event) {
    if (vFrame)
        output_rect = vFrame -> calcSize(this -> rect());
    else
        output_rect = this -> rect();

    QWidget::resizeEvent(event);
    emit resized();
}
