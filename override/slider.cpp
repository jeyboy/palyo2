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

//TODO: draw text by QStaticText
void Slider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    if (!Settings::instance() -> isMetricShow())
        return;

    QPainter p(this);
    p.save();

    p.setPen(QColor::fromRgb(0, 0, 0));
    QRect rect = this -> geometry();
    QString strNum;

    double limit, temp = 0, step = ((double)maximum()) / tickInterval();
    int multiplyer = 0;

    if (orientation() == Qt::Horizontal) {
        rect.moveLeft(rect.left() + margin);
        rect.setWidth(rect.width() - margin);

        while(temp < 16) {
            multiplyer++;
            temp = ((float)(rect.width())) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.width() / step) == 0 ? rect.width() - step : rect.width();
        int bottom = rect.bottom() - 7, h = (rect.height() / 3) - 3;
        double val = multiplyer;

        for(double pos = step; pos < limit; pos += step, val += multiplyer) {
            strNum = QString::number(val);
            p.drawLine(pos, bottom - h, pos, bottom);
            if (position_slider)
                p.drawText(pos - 7 * strNum.length() , bottom, strNum);
        }

        if (position_slider) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(margin, rect.y(), rect.width() - margin - 1, 3);
                p.fillRect(margin, rect.y(), (rect.width() - margin - 1) * pos, 3, fillColor);
            }
        }
    } else {
        rect.moveTop(rect.top() + margin);
        rect.setHeight(rect.height() - margin);

        while(temp < 16) {
            multiplyer++;
            temp = ((float)(rect.height())) / (step / multiplyer);
        }

        step = temp;
        limit = (rect.height() / step) == 0 ? rect.height() - step : rect.height();
        int temp, left = rect.left() + 4, w = (rect.width() / 3) - 3;
        double val = multiplyer;

        for(double pos = step - margin; pos < limit; pos += step, val += multiplyer) {
            strNum = QString::number(val);
            temp = rect.height() - pos;
            p.drawLine(left, temp, left + w, temp);
            if (position_slider)
                p.drawText(left, temp + 10, strNum);
        }

        if (position_slider) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(rect.x(), margin, 3, rect.height() - margin - 1);
                p.fillRect(rect.x(), rect.height(), 3, -((rect.height() - margin - 1) * pos), fillColor);
            }
        }
    }

    p.restore();
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
