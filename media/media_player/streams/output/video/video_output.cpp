#include "video_output.h"
#include "media/media_player/media_player.h"

VideoOutput::VideoOutput(int width, int height, QWidget* parent) : OutputContainer(parent) {
    resize(width, height);

    screen = new GLOutput(this);

    QWidget * bottomPanel = new QWidget(this);
    bottomPanel -> setMaximumHeight(60);
    bottomPanel -> setMinimumHeight(60);

    QHBoxLayout * panelLayout = new QHBoxLayout(this);
    bottomPanel -> setLayout(panelLayout);

    QPushButton * play = new QPushButton(QIcon(":play"), "", this);
    play -> setMaximumSize(40, 50);
    connect(play, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(play()));
    bottomPanel -> layout() -> addWidget(play);

    QPushButton * pause = new QPushButton(QIcon(":pause"), "", this);
    pause -> setMaximumSize(40, 50);
    connect(pause, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(pause()));
    bottomPanel -> layout() -> addWidget(pause);

    QPushButton * stop = new QPushButton(QIcon(":stop"), "", this);
    stop -> setMaximumSize(40, 50);
    connect(stop, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(stop()));
    bottomPanel -> layout() -> addWidget(stop);

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
    if (frame -> image == 0) {
        close();
        return;
    }

    screen -> setFrame(frame);
}
