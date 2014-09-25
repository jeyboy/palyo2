#include "hardware_render.h"

#include <QDebug>
#include <QPainter>

HardwareRender::HardwareRender(QWidget* parent) : RenderInterface(parent) {
    init = true;

    setAutoFillBackground(true);
}

HardwareRender::~HardwareRender() {

}

void HardwareRender::paintEvent(QPaintEvent * event) {
    Q_UNUSED(event);
//    QWidget::paintEvent(event);

    QPainter p(this);
//    p.fillRect(this -> rect(), Qt::black);

    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
    p.setRenderHint(QPainter::Antialiasing, 1);

    mutex.lock();
    p.drawImage(output_rect, *vFrame -> image);
    mutex.unlock();
    redrawed();
}
