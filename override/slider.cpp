#include "slider.h"
#include "qdebug.h"

Slider::Slider(QWidget * parent) : QSlider(parent) {
//    setToolTipDuration(1000);

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

    if (!Settings::instance() -> isMetricShow())
        return;

    QPainter p(this);

    p.save();

    p.setPen(QColor::fromRgb(0, 0, 0));
    QRect rect = geometry();

    double limit, temp = 0, step = ((double)maximum()) / tickInterval();
    int multiplyer = 0, flag = Qt::AlignVertical_Mask | Qt::AlignHCenter;

    if (orientation() == Qt::Horizontal) {
        while(temp < 16) {
            multiplyer++;
            temp = ((float)(rect.width())) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.width() / step) == 0 ? rect.width() - step : rect.width();
        int bottom = rect.bottom() - 7, h = (rect.height() / 3) - 3;

        for(double pos = step; pos < limit; pos += step) {
            p.drawLine(pos, bottom - h, pos, bottom);
        }

        if (multiplyer > 1) {
            flag = Qt::AlignHCenter | Qt::AlignVCenter;
//            p.drawText(4, rect.top() + 18, "x " + QString::number(multiplyer));
        }
    } else {
        while(temp < 20) {
            multiplyer++;
            temp = ((float)(rect.height())) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.height() / step) == 0 ? rect.height() - step : rect.height();
        int right = rect.right() - 7, w = (rect.width() / 3) - 3;

        for(double pos = step; pos < limit; pos += step) {
            p.drawLine(right - w, pos, right, pos);
        }
    }

    if (multiplyer > 1) {
//            p.drawText(rect.left() + 4 + (multiplyer < 10 ? 3 : 0), (int)(limit - step / 2) + 2, "x" + QString::number(multiplyer));

        QStyleOptionSlider opt;
        initStyleOption(&opt);

        QStyle *styl = style();
        QRect rectHandle = styl -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, NULL);
        p.drawText(rectHandle, flag, QString::number(multiplyer));
    }

    p.restore();

//    int avl=styl->pixelMetric(QStyle::PM_SliderSpaceAvailable, &opt, this);
}

// mouse tracking required :(
//void Slider::mouseMoveEvent(QMouseEvent * ev) {
//    qDebug() << "hudo";
//    QPointF p = ev -> localPos();
//    QToolTip::showText(QString::number(p.x()));
//}