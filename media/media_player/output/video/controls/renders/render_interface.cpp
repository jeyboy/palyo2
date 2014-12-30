#include "render_interface.h"

RenderInterface::RenderInterface(int & redrawCounter)
  : init(false)
  , vFrame(0)
  , redrawRef(redrawCounter) {
}

RenderInterface::~RenderInterface() {
    mutex.lock();
    delete vFrame;
    mutex.unlock();
}

void RenderInterface::setFrame(VideoFrame * frame) {
    if (frame && !frame -> skip()) {
        mutex.lock();
        delete vFrame;
        vFrame = frame;
        mutex.unlock();
    }
}

void RenderInterface::resize(QRect rect) {
    if (vFrame)
        output_rect = vFrame -> calcSize(rect);
    else
        output_rect = rect;
}
