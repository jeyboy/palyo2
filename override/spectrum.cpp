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

void Spectrum::paintEvent(QPaintEvent *event) {
    QToolBar::paintEvent(event);

    QPainter painter(this);
    painter.save();

    if (Settings::instance() -> getSpectrumCombo()) {
        paintCombo(painter);
    } else {
        paintDuo(painter);
    }

    painter.restore();
}

void Spectrum::paintCombo(QPainter & painter) {
    int offset = isMovable() ? 10 : 0, padd = 2;
    float peak, accumulate = padd + offset;
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

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        rect.setCoords(accumulate, height() - 6 - peak, (accumulate + bar_width), height() - 6);
        painter.fillRect(rect, g);
        painter.drawRect(rect);
        accumulate += bar_width + padd;
    }
}

void Spectrum::paintDuo(QPainter & painter) {
    int offset = isMovable() ? 10 : 0, padd = 2;
    float peak, peak2, accumulate = padd + offset;
    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
    float bar_height = workHeight() + 3;
    QRectF rect;

    QLinearGradient g(bar_width / 2, 0, bar_width / 2, bar_height);

    if (Settings::instance() -> getMonocolorSpectrum()) {
        g.setColorAt(0, Settings::instance() -> getSpectrumColor2());
        g.setColorAt(1, Settings::instance() -> getSpectrumColor());
    } else {
        g.setColorAt(0.1, Qt::red);
        g.setColorAt(0.5, Qt::yellow);
        g.setColorAt(0.6, Qt::yellow);
        g.setColorAt(1, Qt::darkGreen);
    }

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        rect.setCoords(accumulate, bar_height - peak, (accumulate + bar_width), bar_height);
        painter.fillRect(rect, g);
        painter.drawRect(rect);

        peak2 = peaks[1][loop1];
        rect.setCoords(accumulate, bar_height + 6, (accumulate + bar_width), bar_height + 6 + peak2);
        painter.fillRect(rect, g);
        painter.drawRect(rect);


        accumulate += bar_width + padd;
    }
}
