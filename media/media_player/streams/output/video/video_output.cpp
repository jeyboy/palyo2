#include "video_output.h"
#include "media/media_player/media_player.h"

#include "override/slider.h"
#include "override/slider_style.h"

VideoOutput::VideoOutput(QObject * parent, int width, int height) : OutputContainer() {
    setMouseTracking(true);
    resize(width, height);

    screen = new GLOutput(this);
    screen -> setMouseTracking(true);
    connect(screen, SIGNAL(closed()), this, SLOT(close()));
    connect(screen, SIGNAL(updated()), this, SLOT(titleUpdate()));
    connect(screen, SIGNAL(updated()), parent, SLOT(nextPict()));

    bottomPanel = new QWidget(this);
    bottomPanel -> setBackgroundRole(QPalette::Shadow);
    bottomPanel -> setAutoFillBackground(true);
    bottomPanel -> setMaximumHeight(60);
    bottomPanel -> setMinimumHeight(60);

    QHBoxLayout * panelLayout = new QHBoxLayout(this);
    bottomPanel -> setLayout(panelLayout);
    bottomPanel -> setVisible(false); // remove later

    QPushButton * play = new QPushButton(QIcon(":play"), "", this);
    play -> setMaximumSize(40, 50);
    play -> setMinimumWidth(40);
    connect(play, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(play()));
    bottomPanel -> layout() -> addWidget(play);

    QPushButton * pause = new QPushButton(QIcon(":pause"), "", this);
    pause -> setMaximumSize(40, 50);
    pause -> setMinimumWidth(40);
    connect(pause, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(pause()));
    bottomPanel -> layout() -> addWidget(pause);

    QPushButton * stop = new QPushButton(QIcon(":stop"), "", this);
    stop -> setMaximumSize(40, 50);
    stop -> setMinimumWidth(40);
    connect(stop, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(stop()));
    bottomPanel -> layout() -> addWidget(stop);

    Slider * slider = new Slider(bottomPanel, true);
    slider -> setStyle(new SliderStyle());
    slider -> setTickInterval(60000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(100, 30);
    bottomPanel -> layout() -> addWidget(slider);


    QVBoxLayout * newLayout = new QVBoxLayout(this);
    newLayout -> addWidget(screen);
    newLayout -> addWidget(bottomPanel);
    newLayout -> setContentsMargins(0, 0, 0, 0);
    setLayout(newLayout);

    show();
    setFocus();
}

VideoOutput::~VideoOutput() {

}

void VideoOutput::setFrame(VideoFrame * frame) {
    screen -> setFrame(frame);
}

void VideoOutput::titleUpdate() {
    setWindowTitle(MediaPlayer::instance() -> info());
}

void VideoOutput::mouseMoveEvent(QMouseEvent * event) {
    QRect windRect = rect();
    windRect.setTop(windRect.bottom() - (windRect.height() / 10));
    if (windRect.contains(event -> x(), event -> y())) {
        bottomPanel -> setVisible(true);
    } else {
        bottomPanel -> setVisible(false);
    }
}
