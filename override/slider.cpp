#include "slider.h"
#include "qdebug.h"
#include "media/player.h"
#include "media/duration.h"
#include "misc/stylesheets.h"

Slider::Slider(QWidget * parent, bool isPositionSlider) : QSlider(parent) {
    setMouseTracking(isPositionSlider);
    position_slider = isPositionSlider;
    fillColor = QColor::fromRgb(0,0,0);
//    setToolTipDuration(1000);

    setStyleSheet(Stylesheets::sliderStyles());
    margin = 4;
}


//TODO: problem with download line start if bar is not movable
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
            temp = ((float)(rect.width() - 2 * margin)) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.width() / step) == 0 ? rect.width() - step : rect.width();
        int bottom = rect.bottom() - 7, h = (rect.height() / 3) - 3;

        for(double pos = step; pos < limit; pos += step) {
            p.drawLine(pos, bottom - h, pos, bottom);
        }

        if (multiplyer > 1) {
            flag = Qt::AlignHCenter | Qt::AlignVCenter;
        }

        if (position_slider) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(margin, rect.y(), rect.width() - 1 - margin * 2, 3);
                p.fillRect(margin, rect.y(), (rect.width() - 1 - margin * 2) * pos, 3, fillColor);
            }
        }

    } else {
        while(temp < 20) {
            multiplyer++;
            temp = ((float)(rect.height() - 2 * margin)) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.height() / step) == 0 ? rect.height() - step : rect.height();
        int right = rect.right() - 7, w = (rect.width() / 3) - 3;

        for(double pos = step; pos < limit; pos += step) {
            p.drawLine(right - w, pos, right, pos);
        }

        if (position_slider) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(rect.x(), margin, 3, rect.height() - 1 - margin * 2);
                p.fillRect(rect.x(), rect.height() - margin, 3, -((rect.height() - 1 - margin * 2) * pos), fillColor);
            }
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

void Slider::mouseMoveEvent(QMouseEvent * ev) {
    if (hasMouseTracking()) {
        QPointF p = ev -> localPos();
        bool show = false;

        int dur;
        if (orientation() == Qt::Vertical) {
            if ((show = (p.y() > margin && p.y() < height() - margin)))
                dur = maximum() *((height() - margin - p.y()) / (height() - 2 * margin));
        } else {
            if ((show = (p.x() > margin && p.x() < width() - margin)))
                dur = maximum() * ((p.x() - margin) / (width() - 2 * margin));
        }

        if (show)
            QToolTip::showText(ev -> globalPos(), Duration::fromMillis(dur));

    }

    QSlider::mouseMoveEvent(ev);
}
