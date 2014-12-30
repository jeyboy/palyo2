#include "render_interface.h"

RenderInterface::RenderInterface(int & redrawCounter)
  : init(false)
  , updateRequired(false)
  , redrawRef(redrawCounter)
  , vFrame(0) {
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
        updateRequired = true;
        mutex.unlock();
        toWidget() -> repaint();
    }
}

void RenderInterface::resize(QRect rect) {
    updateRequired = true;
    if (vFrame)
        output_rect = vFrame -> calcSize(rect);
    else
        output_rect = rect;
}
