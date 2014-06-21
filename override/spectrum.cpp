#include "spectrum.h"
#include "media/player.h"

Spectrum::Spectrum(QWidget *parent) : QToolBar("Spectrum", parent) {
    setObjectName("tool_Spectrum");
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    connect(Player::instance(), SIGNAL(spectrumChanged(QList<QVector<int> >)), this, SLOT(dataUpdated(QList<QVector<int> >)));
    bandCountChanged(Settings::instance() -> getSpectrumBarsCount());
    heightChanged(Settings::instance() -> getSpectrumHeight());
}

Spectrum::~Spectrum() {

}

int Spectrum::workHeight() {
    if (Settings::instance() -> getSpectrumCombo())
        return height() - 10;
    else
        return (height() - 18) / 2;
}

void Spectrum::bandCountChanged(int newCount) {
    Player::instance() -> setSpectrumBandsCount((bars_count = newCount));
    peaks = Player::instance() -> getDefaultSpectrum();
}

void Spectrum::heightChanged(int newHeight) {
    setFixedHeight(newHeight);
    setMinimumWidth(200);
    Player::instance() -> setSpectrumHeight(workHeight());
}

void Spectrum::dataUpdated(QList<QVector<int> > bars) {
//    qDebug() << bars;
    peaks = bars;
    repaint();
}

int Spectrum::paddWidth() {
    return 2;
}

void Spectrum::paintEvent(QPaintEvent *event) {

    QToolBar::paintEvent(event);

    if (Settings::instance() -> getSpectrumCombo()) {
        paintCombo();
    } else {
        paintDuo();
    }
}

void Spectrum::paintCombo() {
    QPainter painter(this);
    painter.save();
    int offset = isMovable() ? 10 : 0, padd = paddWidth();
    double peak, accumulate = padd + offset;
    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
    QRectF rect;

    QLinearGradient g(bar_width / 2, 0, bar_width / 2, workHeight());

    if (Settings::instance() -> getMonocolorSpectrum()) {
        g.setColorAt(0, Settings::instance() -> getSpectrumColor2());
        g.setColorAt(1, Settings::instance() -> getSpectrumColor());
    } else {
        g.setColorAt(0.1, Qt::red);
        g.setColorAt(0.5, Qt::yellow);
        g.setColorAt(0.6, Qt::yellow);
        g.setColorAt(1, Qt::darkGreen);
    }

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        rect.setCoords(accumulate, height() - 6 - peak, (accumulate + bar_width), height() - 6);
        painter.fillRect(rect, g);
        painter.drawRect(rect);
        accumulate += bar_width + padd;
    }
    painter.restore();
}

void Spectrum::paintDuo() {
    QPainter painter(this);
    painter.save();
    int pairs = (peaks.length() + 1) / 2;
    int offset = isMovable() ? 10 : 0;
    double peak, peak2, temp_acc, accumulate = offset, beetween_space = 10;
    float bar_width = ((float)width() - offset - ((pairs - 1) * beetween_space))/ pairs / bars_count;
    float bar_height = workHeight() + 3, first_bar_place =  bar_height + 3, sec_bar_place = bar_height + 9;
    QRectF rect;

    QLinearGradient g(bar_width / 2, 0, bar_width / 2, bar_height);
    QLinearGradient gg(bar_width / 2, sec_bar_place + bar_height, bar_width / 2, sec_bar_place);

    if (Settings::instance() -> getMonocolorSpectrum()) {
        g.setColorAt(0, Settings::instance() -> getSpectrumColor2());
        g.setColorAt(1, Settings::instance() -> getSpectrumColor());

        gg.setColorAt(0, Settings::instance() -> getSpectrumColor2());
        gg.setColorAt(1, Settings::instance() -> getSpectrumColor());
    } else {
        g.setColorAt(0.1, Qt::red);
        g.setColorAt(0.5, Qt::yellow);
        g.setColorAt(0.6, Qt::yellow);
        g.setColorAt(1, Qt::darkGreen);

        gg.setColorAt(0.1, Qt::red);
        gg.setColorAt(0.5, Qt::yellow);
        gg.setColorAt(0.6, Qt::yellow);
        gg.setColorAt(1, Qt::darkGreen);
    }

    for(int pair = 0; pair < peaks.length(); pair += 2) {
        if (peaks.length() > pair + 1) {
            for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                temp_acc = (accumulate + bar_width);

                peak = peaks[pair][loop1];
                rect.setCoords(accumulate, first_bar_place - peak, temp_acc, first_bar_place);
                painter.fillRect(rect, g);
                painter.drawRect(rect);

                peak2 = peaks[pair + 1][loop1];
                rect.setCoords(accumulate, sec_bar_place, temp_acc, sec_bar_place + peak2);
                painter.fillRect(rect, gg);
                painter.drawRect(rect);

                accumulate = temp_acc;
            }
        } else {
            for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                temp_acc = (accumulate + bar_width);

                peak = peaks[pair][loop1];
                rect.setCoords(accumulate, first_bar_place - peak, temp_acc, first_bar_place);
                painter.fillRect(rect, g);
                painter.drawRect(rect);

                accumulate = temp_acc;
            }
        }
        accumulate += beetween_space;
    }
    painter.restore();
}



//void Spectrum::paintDuo() {
//    QPainter painter(this);
//    painter.save();
//    int offset = isMovable() ? 10 : 0, padd = paddWidth();
//    double peak, peak2, temp_acc, accumulate = padd + offset;
//    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
//    float bar_height = workHeight() + 3, first_bar_place =  bar_height + 3, sec_bar_place = bar_height + 9;
//    QRectF rect;

//    QLinearGradient g(bar_width / 2, 0, bar_width / 2, bar_height);
//    QLinearGradient gg(bar_width / 2, sec_bar_place + bar_height, bar_width / 2, sec_bar_place);

//    if (Settings::instance() -> getMonocolorSpectrum()) {
//        g.setColorAt(0, Settings::instance() -> getSpectrumColor2());
//        g.setColorAt(1, Settings::instance() -> getSpectrumColor());

//        gg.setColorAt(0, Settings::instance() -> getSpectrumColor2());
//        gg.setColorAt(1, Settings::instance() -> getSpectrumColor());
//    } else {
//        g.setColorAt(0.1, Qt::red);
//        g.setColorAt(0.5, Qt::yellow);
//        g.setColorAt(0.6, Qt::yellow);
//        g.setColorAt(1, Qt::darkGreen);

//        gg.setColorAt(0.1, Qt::red);
//        gg.setColorAt(0.5, Qt::yellow);
//        gg.setColorAt(0.6, Qt::yellow);
//        gg.setColorAt(1, Qt::darkGreen);
//    }

//    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
//        temp_acc = (accumulate + bar_width);

//        peak = peaks[0][loop1];
//        rect.setCoords(accumulate, first_bar_place - peak, temp_acc, first_bar_place);
//        painter.fillRect(rect, g);
//        painter.drawRect(rect);

//        peak2 = peaks[1][loop1];
//        rect.setCoords(accumulate, sec_bar_place, temp_acc, sec_bar_place + peak2);
//        painter.fillRect(rect, gg);
//        painter.drawRect(rect);

//        accumulate = temp_acc + padd;
//    }
//    painter.restore();
//}
