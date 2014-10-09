#include "hardware_render.h"

#include <QDebug>
#include <QPainter>

HardwareRender::HardwareRender(QWidget* parent) : RenderInterface(parent) {
    setAutoFillBackground(false);
}

HardwareRender::~HardwareRender() {

}

void HardwareRender::setQuality(const enum RenderQuality & quality) {
    switch(quality) {
        case RenderQuality::best : {

        break;}
        case RenderQuality::low : {

        break;}
        default : {

        }
    };
}

void HardwareRender::paintEvent(QPaintEvent * event) {
    if (vFrame == 0) return;

    if (!init) {
        init = true;
        output_rect = vFrame -> calcSize(this -> rect());
    }

    Q_UNUSED(event);
//    QWidget::paintEvent(event);

    QPainter p(this);
    p.fillRect(rect(), Qt::black);

    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
    p.setRenderHint(QPainter::Antialiasing, 1);

    mutex.lock();
    p.drawImage(output_rect, *vFrame -> asImage());
    mutex.unlock();
    redrawed();
}
