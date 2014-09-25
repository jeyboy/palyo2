#include "control_panel.h"
#include "media/media_player/media_player.h"

ControlPanel::ControlPanel(QWidget * parent) : QWidget(parent) {
//    setAttribute(Qt::WA_OpaquePaintEvent);
//    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_LayoutUsesWidgetRect);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    setBackgroundRole(QPalette::Shadow);
    setAutoFillBackground(true);
    setMaximumHeight(60);
    setMinimumHeight(60);

    QHBoxLayout * panelLayout = new QHBoxLayout(this);
    setLayout(panelLayout);
    setVisible(false); // remove later

    QPushButton * play = new QPushButton(QIcon(":play"), "", this);
    play -> setMaximumSize(40, 50);
    play -> setMinimumWidth(40);
    connect(play, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(play()));
    layout() -> addWidget(play);

    QPushButton * pause = new QPushButton(QIcon(":pause"), "", this);
    pause -> setMaximumSize(40, 50);
    pause -> setMinimumWidth(40);
    connect(pause, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(pause()));
    layout() -> addWidget(pause);

    QPushButton * stop = new QPushButton(QIcon(":stop"), "", this);
    stop -> setMaximumSize(40, 50);
    stop -> setMinimumWidth(40);
    connect(stop, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(stop()));
    layout() -> addWidget(stop);

    timer = new QLabel(this);
    layout() -> addWidget(timer);

    slider = new Slider(this, true);
    slider -> setStyle(new SliderStyle());
    slider -> setTickInterval(60000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(100, 30);

    slider -> setMaximum(MediaPlayer::instance() -> __duration());
    connect(MasterClock::instance(), SIGNAL(__positionUpdated(int)), this, SLOT(sliderUpdate(int)));
    connect(slider, SIGNAL(valueChanged(int)), MediaPlayer::instance(), SLOT(seekMillis(int)));

    layout() -> addWidget(slider);
}

QRect ControlPanel::getRegion() const {
    return region;
}

void ControlPanel::setRegion(QRect rect) {
//    rect.setBottom(rect.top() + minimumHeight());
    rect.setTop(rect.bottom() - minimumHeight() + 1);
    setGeometry((region = rect));
}

void ControlPanel::sliderUpdate(int pos) {
    slider -> blockSignals(true);
    slider -> setValue(pos);
    slider -> blockSignals(false);

    QString temp = MediaPlayer::instance() -> info();
    timer -> setText(temp);
}