#include "video_output.h"
#include "media/media_player/media_player.h"

VideoOutput::VideoOutput(QObject * parent, RenderType type, int width, int height) : OutputContainer()
    , screen(0) {

    setMouseTracking(true);
    resize(width, height);

    QVBoxLayout * newLayout = new QVBoxLayout(this);
    newLayout -> setContentsMargins(0, 0, 0, 0);
    setLayout(newLayout);
    setRender(type);

    panel -> show();
    panel -> raise();

    show();
    setFocus();
}

VideoOutput::~VideoOutput() {

}

void VideoOutput::setRender(RenderType type) {
    if (screen) {
        layout() -> removeWidget(screen);
        delete screen;
        screen = 0;
    }

    switch(type) {
        case gl: {
            screen = new GLRender(this);
            break;
        }
        default: {
            screen = new HardwareRender(this);
            break;
        }
    }

    screen -> setStyleSheet("QWidget { border-style: outset;  border-width: 2px;  border-radius: 10px;  border-color: beige;}");
    connect(screen, SIGNAL(updated()), this, SLOT(titleUpdate()));
    screen -> setMouseTracking(true);
    layout() -> addWidget(screen);
    panel -> setRegion(rect());
}

void VideoOutput::titleUpdate() {
    QString temp = MediaPlayer::instance() -> info();
    setWindowTitle(temp);
}

void VideoOutput::setFrame(void * frame) {
    screen -> setFrame((VideoFrame *)frame);
}

void VideoOutput::mouseMoveEvent(QMouseEvent * event) {
    if (panel -> getRegion().contains(event -> x(), event -> y())) {
        panel -> setVisible(true);
    } else {
        panel -> setVisible(false);
    }
}

void VideoOutput::resizeEvent(QResizeEvent * event) {
    panel -> setRegion(rect());
    OutputContainer::resizeEvent(event);
}

//void VideoOutput::paintEvent(QPaintEvent * event) {
//    OutputContainer::paintEvent(event);
//    QPainter painter(this);
//    panel -> render(&painter, panel -> getRegion().topLeft());
//}
