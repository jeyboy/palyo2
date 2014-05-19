#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QPaintEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionSlider>

class Slider : public QSlider {
    Q_OBJECT
public:
    Slider(QWidget * parent = 0);

protected:
    void paintEvent(QPaintEvent *);
public:

};

#endif // SLIDER_H
