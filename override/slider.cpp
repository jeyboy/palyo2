#include "slider.h"
#include "qdebug.h"

Slider::Slider(QWidget * parent) : QSlider(parent) {
    setStyleSheet(QString(
                                              "QSlider::groove {"
                                                "border: 1px solid #bbb;"
                                                "border-radius: 2px;"
                                              "}"

                                              "QSlider::groove:horizontal {"
                                                "height: 18px;"
                                              "}"

                                              "QSlider::groove:vertical {"
                                                "width: 18px;"
                                              "}"

                                              "QSlider::sub-page:horizontal {"
                                                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000, stop: 1 #777);"
                                                "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 1, stop: 0 #777, stop: 1 #fff);"
                                                "border: 1px solid #777;"
                                                "border-radius: 2px;"
                                              "}"

                                              "QSlider::add-page:horizontal {"
                                                "background: #fff;"
                                                "border: 1px solid #777;"
                                                "border-radius: 2px;"
                                              "}"

                                              "QSlider::sub-page:vertical {"
                                                  "background: #fff;"
                                                  "border: 1px solid #777;"
                                                  "border-radius: 2px;"
                                              "}"

                                              "QSlider::add-page:vertical {"
                                                  "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000, stop: 1 #777);"
                                                  "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 1, stop: 0 #777, stop: 1 #fff);"
                                                  "border: 1px solid #777;"
                                                  "border-radius: 2px;"
                                              "}"

                                              "QSlider::handle {"
                                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc);"
                                                "border: 1px solid #777;"
                                                "margin: 0 -1px;"
                                                "border-radius: 2px;"
                                              "}"

                                              "QSlider::handle:horizontal {"
                                                "width: 12px;"
                                              "}"

                                              "QSlider::handle:vertical {"
                                                "height: 12px;"
                                              "}"

                                              "QSlider::handle:hover {"
                                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ddd);"
                                                "border: 1px solid #444;"
                                                "border-radius: 2px;"
                                              "}"

                                              "QSlider::sub-page:disabled {"
                                                "background: #bbb;"
                                                "border-color: #999;"
                                              "}"

                                              "QSlider::add-page:disabled {"
                                                "background: #eee;"
                                                "border-color: #999;"
                                              "}"

                                              "QSlider::handle:disabled {"
                                                "background: #eee;"
                                                "border: 1px solid #aaa;"
                                                "border-radius: 2px;"
                                              "}"
    ));
}

void Slider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    QPainter p(this);

    p.save();

    p.setPen(QColor::fromRgb(0, 0, 0));
    QRect rect = geometry();

    float limit, step = ((float)maximum()) / tickInterval();

    if (orientation() == Qt::Horizontal) {
        step = ((float)rect.width()) / step;
        limit = (rect.width() / step) == 0 ? rect.width() - step : rect.width();

        for(float pos = step; pos < limit; pos += step) {
            p.drawLine(pos, rect.top() + 4, pos, rect.bottom() - 7);
        }
    } else {
        step = ((float)rect.height()) / step;
        limit = (rect.height() / step) == 0 ? rect.height() - step : rect.height();

        for(float pos = step; pos < limit; pos += step) {
            p.drawLine(rect.left() + 4, pos, rect.right() - 7, pos);
        }
    }

    p.restore();
}
