#include "control_panel.h"
#include "media/media_player/media_player.h"

ControlPanel::ControlPanel(MasterClock * clock, QWidget * parent) : QFrame(parent), clock(clock) {
//    setAttribute(Qt::WA_OpaquePaintEvent);
//    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_LayoutUsesWidgetRect);

    setWindowFlags(Qt::WindowStaysOnTopHint);

//    setBackgroundRole(QPalette::Shadow);
//    setAutoFillBackground(true);
    setMaximumHeight(60);
    setMinimumHeight(60);

    setStyleSheet(
                "ControlPanel {"
                "   background: transparent #333;"
                "   border-radius: 16px;"
                "}"
                "QPushButton {"
                "   border-radius: 16px;"
                "   border: 2px solid #000;"
                "   background-color: #eee;"
                "}"
                "QPushButton:hover {"
                "   background-color: #bbb;"
                "}"
//                "QPushButton:pressed {"
//                "   background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
//                "}"
//                QPushButton:on {
//                        background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #5AA72D,
//                        stop :   0.5 #B3E296, stop :   0.55 #B3E296, stop :   1.0 #f5f9ff);
//                        padding-top: 2px;
//                        padding-left: 3px;
//                }

//                QPushButton:disabled {
//                        background: transparent #e5e9ee;
//                        padding-top: 2px;
//                        padding-left: 3px;
//                        color: black;
//                }
                "QLabel {"
                "   color: #fff;"
                "}"
    );

    QHBoxLayout * panelLayout = new QHBoxLayout(this);
    setLayout(panelLayout);
    setVisible(false); // remove later

    QPushButton * play = new QPushButton(QIcon(":play"), "", this);
    play -> setMaximumSize(40, 50);
    play -> setMinimumWidth(40);
    connect(play, SIGNAL(clicked()), (MediaPlayer *)clock -> mediaPlayer(), SLOT(play()));
    layout() -> addWidget(play);

    QPushButton * pause = new QPushButton(QIcon(":pause"), "", this);
    pause -> setMaximumSize(40, 50);
    pause -> setMinimumWidth(40);
    connect(pause, SIGNAL(clicked()), (MediaPlayer *)clock -> mediaPlayer(), SLOT(pause()));
    layout() -> addWidget(pause);

    QPushButton * stop = new QPushButton(QIcon(":stop"), "", this);
    stop -> setMaximumSize(40, 50);
    stop -> setMinimumWidth(40);
    connect(stop, SIGNAL(clicked()), (MediaPlayer *)clock -> mediaPlayer(), SLOT(stop()));
    layout() -> addWidget(stop);

    timer = new QLabel(this);
    layout() -> addWidget(timer);

    slider = new Slider(this, true);
    slider -> setStyle(new SliderStyle());
    slider -> setTickInterval(60000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(100, 30);

    slider -> setMaximum(((MediaPlayer *) clock -> mediaPlayer()) -> durationMillis());
    connect((MediaPlayer *)clock -> mediaPlayer(), SIGNAL(positionChangedMillis(int)), this, SLOT(sliderUpdate(int)));
    connect(slider, SIGNAL(valueChanged(int)), (MediaPlayer *)clock -> mediaPlayer(), SLOT(seekMillis(int)));

    layout() -> addWidget(slider);
}

QRect ControlPanel::getRegion() const {
    return region;
}

void ControlPanel::setRegion(QRect rect) {
//    rect.setBottom(rect.top() + minimumHeight() + 10);
    rect.setTop(rect.bottom() - (minimumHeight() + 10));
    rect.setLeft(10); rect.setRight(rect.right() - 10);
    setGeometry((region = rect));
}

void ControlPanel::sliderUpdate(int pos) {
    slider -> blockSignals(true);
    slider -> setValue(pos);
    slider -> blockSignals(false);

    QString temp = ((MediaPlayer *) clock -> mediaPlayer()) -> info();
    timer -> setText(temp);
}

void ControlPanel::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
