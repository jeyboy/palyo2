#include "hardware_render.h"

#include <QDebug>
#include <QPainter>

HardwareRender::HardwareRender(QWidget* parent) : RenderInterface(parent) {
    setAutoFillBackground(false);
}

HardwareRender::~HardwareRender() {

}

void HardwareRender::initializeGL() {
    RenderInterface::initializeGL();

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &HardwareRender::cleanup, Qt::DirectConnection);
}

void HardwareRender::cleanup() {
    qDebug() << "HARDWARE CLEANUP";
    makeCurrent();
    ////////////
    doneCurrent();
}

//TODO: add quality settings
void HardwareRender::setQuality(const Quality & quality) {
    switch(quality) {
        case RenderInterface::best : {

        break;}
        case RenderInterface::low : {

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
