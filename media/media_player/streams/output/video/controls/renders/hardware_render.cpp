#include "hardware_render.h"

#include <QDebug>

RenderInterface::RenderInterface(QWidget* parent) : QWidget(parent)
  , vFrame(0)
  , init(true) {

    setAutoFillBackground(true);
}

void HardwareRender::paintEvent(QPaintEvent * event) {
    Q_UNUSED(event);
//    QWidget::paintEvent(event);

    QPainter p(this);
//    p.fillRect(this -> rect(), Qt::black);

    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
    p.setRenderHint(QPainter::Antialiasing, 1);

    mutex.lock();
    p.drawImage(output_rect, *frame -> image);
    mutex.unlock();
    redrawed();
}
