#include "spectrum.h"
#include "media/player.h"

Spectrum::Spectrum(QWidget *parent) : QToolBar("Spectrum", parent) {
    setObjectName("tool_Spectrum");

    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    setFixedHeight(100);
    connect(Player::instance(), SIGNAL(spectrumChanged(QList<int>)), this, SLOT(dataUpdated(QList<int>)));
    Player::instance() -> setSpectrumBandsCount((bars_count = 40));
    Player::instance() -> setSpectrumHeight(height() - 10);
}

Spectrum::~Spectrum() {

}

void Spectrum::dataUpdated(QList<int> bars) {
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

    for(int loop1 = 0; loop1 < peaks.length(); loop1++) {
        peak = peaks[loop1];
        if (peak < 0) peak = -1;
        rect.setCoords(accumulate, height() - peak, (accumulate + bar_width), height() - 4);
        painter.fillRect(rect, Qt::SolidPattern);
        accumulate += bar_width + padd;
    }

    painter.restore();
}
