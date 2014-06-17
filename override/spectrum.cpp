#include "spectrum.h"
#include "media/player.h"

Spectrum::Spectrum(QWidget *parent) : QToolBar("Spectrum", parent) {
    setObjectName("tool_Spectrum");
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    connect(Player::instance(), SIGNAL(spectrumChanged(QList<int>)), this, SLOT(dataUpdated(QList<int>)));
    bandCountChanged(Settings::instance() -> getSpectrumBarsCount());
    heightChanged(Settings::instance() -> getSpectrumHeight());
}

Spectrum::~Spectrum() {

}

void Spectrum::bandCountChanged(int newCount) {
    Player::instance() -> setSpectrumBandsCount((bars_count = newCount));
}

void Spectrum::heightChanged(int newHeight) {
    setFixedHeight(newHeight);
    setMinimumWidth(200);
    Player::instance() -> setSpectrumHeight(height() - 10);
}

void Spectrum::dataUpdated(QList<int> bars) {
//    qDebug() << bars;
    peaks = bars;
    repaint();
}

void Spectrum::paintEvent(QPaintEvent *event) {
    QToolBar::paintEvent(event);

    QPainter painter(this);
    painter.save();

    int offset = isMovable() ? 10 : 0, padd = 2;
    float peak, accumulate = padd + offset;
    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
    QRectF rect;

    bool monoColor = Settings::instance() -> getMonocolorSpectrum();
    QLinearGradient g(bar_width/2,0,bar_width/2,90);

    if (monoColor) {
        g.setColorAt(0, Settings::instance() -> getSpectrumColor());
    } else {
        g.setColorAt(0, Qt::red);
        g.setColorAt(0.4, Qt::yellow);
        g.setColorAt(0.5, Qt::yellow);
        g.setColorAt(1, Qt::darkGreen);
    }

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks.length(); loop1++) {
        peak = peaks[loop1];
        if (peak < 0) peak = -1;
        rect.setCoords(accumulate, height() - peak, (accumulate + bar_width), height() - 4);
        painter.fillRect(rect, g);
//        painter.fillRect(rect, Qt::SolidPattern); // for monochrome fill
        painter.drawRect(rect);
        accumulate += bar_width + padd;
    }

    painter.restore();
}
