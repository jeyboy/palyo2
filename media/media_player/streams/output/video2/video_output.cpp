#include "video_output.h"
#include "media/media_player/media_player.h"

VideoOutput::VideoOutput(QObject * parent, int width, int height) : OutputContainer() {
    setMouseTracking(true);
    resize(width, height);

    screen = new GLOutput(this);
    screen -> setMouseTracking(true);
    connect(screen, SIGNAL(updated()), this, SLOT(titleUpdate()));

    QVBoxLayout * newLayout = new QVBoxLayout(this);
    newLayout -> addWidget(screen);
    newLayout -> setContentsMargins(0, 0, 0, 0);
    setLayout(newLayout);

    panel -> setRegion(rect());
    panel -> show();
    screen -> lower();

    show();
    setFocus();
}

VideoOutput::~VideoOutput() {

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
