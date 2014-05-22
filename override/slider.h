#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QPaintEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QToolTip>

#include "misc/settings.h"

class Slider : public QSlider {
    Q_OBJECT
public:
    Slider(QWidget * parent = 0);

protected:
    void paintEvent(QPaintEvent *);
//    void mouseMoveEvent(QMouseEvent *);
public:

};

#endif // SLIDER_H
