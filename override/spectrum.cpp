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
   return height() - 10;
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

//    paintCombo();
    paintDuo();
}

void Spectrum::paintCombo() {
    QPainter painter(this);
    painter.save();

    int offset = isMovable() ? 10 : 0, padd = 2;
    float peak, accumulate = padd + offset;
    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
    QRectF rect;

    bool monoColor = Settings::instance() -> getMonocolorSpectrum();
    QLinearGradient g(bar_width/2, 0,bar_width/2, workHeight());

    if (monoColor) {
        g.setColorAt(0, Settings::instance() -> getSpectrumColor());
    } else {
        g.setColorAt(0.1, Qt::red);
        g.setColorAt(0.5, Qt::yellow);
        g.setColorAt(0.6, Qt::yellow);
        g.setColorAt(1, Qt::darkGreen);
    }

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        if (peak < 0) peak = -1;
        rect.setCoords(accumulate, height() - peak, (accumulate + bar_width), height() - 4);
        painter.fillRect(rect, g);
        painter.drawRect(rect);
        accumulate += bar_width + padd;
    }

    painter.restore();
}

void Spectrum::paintDuo() {
    QPainter painter(this);
    painter.save();

    int offset = isMovable() ? 10 : 0, padd = 2;
    float peak, peak2, accumulate = padd + offset;
    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
    float bar_height = (workHeight() - 2) / 2;
    QRectF rect;

    bool monoColor = Settings::instance() -> getMonocolorSpectrum();
    QLinearGradient g(bar_width/2, 0,bar_width/2, bar_height);

    if (monoColor) {
        g.setColorAt(0, Settings::instance() -> getSpectrumColor());
    } else {
        g.setColorAt(0.1, Qt::red);
        g.setColorAt(0.5, Qt::yellow);
        g.setColorAt(0.6, Qt::yellow);
        g.setColorAt(1, Qt::darkGreen);
    }

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        if (peak < 0) peak = -1;
        rect.setCoords(accumulate, bar_height - peak, (accumulate + bar_width), bar_height);
        painter.fillRect(rect, g);
        painter.drawRect(rect);

        peak2 = peaks[1][loop1];
        if (peak2 < 0) peak2 = -1;
        rect.setCoords(accumulate, height() - peak2, (accumulate + bar_width), height() - 4);
        painter.fillRect(rect, g);
        painter.drawRect(rect);


        accumulate += bar_width + padd;
    }

    painter.restore();
}
