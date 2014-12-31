#include "stub_render.h"

StubRender::StubRender(int & redrawCounter, QWidget* parent) : RenderInterface(redrawCounter), QWidget(parent) {
    setAutoFillBackground(false);
    init = true;
}

StubRender::~StubRender() {

}

void StubRender::paintEvent(QPaintEvent * event) {
    if (vFrame == 0 || !updateRequired) return;

    QWidget::paintEvent(event);

//    QPainter p(this);
//    p.fillRect(rect(), Qt::black);

//    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
//    p.setRenderHint(QPainter::Antialiasing, 1);

//    mutex.lock();
//    p.drawImage(output_rect, *vFrame -> asImage());
//    mutex.unlock();
    redrawed();
}
