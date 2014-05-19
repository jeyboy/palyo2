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

    float limit, temp = 0, step = ((float)maximum()) / tickInterval();
    int multiplyer = 0, flag = Qt::AlignVertical_Mask | Qt::AlignHCenter;

    if (orientation() == Qt::Horizontal) {
        while(temp < 16) {
            multiplyer++;
            temp = ((float)rect.width()) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.width() / step) == 0 ? rect.width() - step : rect.width();

        for(float pos = step; pos < limit; pos += step) {
            p.drawLine(pos, rect.top() + 4, pos, rect.bottom() - 7);
        }

        if (multiplyer > 1) {
            flag = Qt::AlignHCenter | Qt::AlignVCenter;
//            p.drawText(4, rect.top() + 18, "x " + QString::number(multiplyer));
        }
    } else {
        while(temp < 20) {
            multiplyer++;
            temp = ((float)rect.height()) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.height() / step) == 0 ? rect.height() - step : rect.height();

        for(float pos = step; pos < limit; pos += step) {
            p.drawLine(rect.left() + 4, pos, rect.right() - 7, pos);
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






//    QStylePainter painter(this);
//    QStyleOptionSlider opt;
//    initStyleOption(&opt);

//    opt.init(this);
//    opt.orientation = orientation();
//    opt.rect = this -> geometry();
//    opt.minimum = 0;
//    opt.maximum = maximum();
//    opt.sliderPosition = value();
////    opt.sliderValue = 20;
//    opt.subControls = QStyle::SC_SliderHandle;
//    painter.drawPrimitive(Control(ComplexControl(QStyle::CC_Slider, opt);
}
