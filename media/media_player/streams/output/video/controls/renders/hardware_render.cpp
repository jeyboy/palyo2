#include "hardware_render.h"

#include <QDebug>

RenderInterface::RenderInterface(QWidget* parent) : QWidget(parent)
  , vFrame(0)
  , init(true) {

}

void RenderInterface::paintEvent(QPaintEvent *) {

}
