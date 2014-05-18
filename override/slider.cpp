#include "slider.h"
#include "qdebug.h"

Slider::Slider(QWidget * parent) : QSlider(parent) {

}

void Slider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    QPainter p(this);

    p.save();

    p.setPen(QColor::fromRgb(0, 0, 0));
    QRect rect = geometry();

    float step = ((float)rect.width()) / (((float)maximum()) / tickInterval());
    float limit = (maximum() % tickInterval()) == 0 ? rect.width() - step : rect.width();

    for(float pos = step; pos < limit; pos += step) {
        p.drawLine(pos, rect.top() + 4, pos, rect.bottom() - 7);
    }

    p.restore();
}
