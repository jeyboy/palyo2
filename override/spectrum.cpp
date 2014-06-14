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

    QLinearGradient g(bar_width/2,0,bar_width/2,90);

    g.setColorAt(0, Qt::red);
    g.setColorAt(0.4, Qt::yellow);
    g.setColorAt(0.5, Qt::yellow);
    g.setColorAt(1, Qt::darkGreen);

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
