#include "gl_output.h"
#include "media/media_player/media_player.h"

#include <QDebug>

GLOutput::GLOutput(QWidget* parent) : QGLWidget(parent)
  , frame(new VideoFrame) {

    bottomPanel = new QWidget(this, Qt::Popup);
    bottomPanel -> setMaximumHeight(100);

    QPushButton * play = new QPushButton(QIcon(":play"), "", this);
    connect(play, SIGNAL(clicked()), MediaPlayer::instance(), SLOT(play()));

    bottomPanel -> layout() -> addWidget(play);

    QVBoxLayout * newLayout = new QVBoxLayout(this);
    newLayout -> addWidget(bottomPanel, 0, Qt::AlignBottom);
    setLayout(newLayout);

//    setWindowFlags(Qt::WindowStaysOnTopHint);
    show();
    setFocus();
//    setWindowFlags((windowFlags() & ~Qt::WindowStaysOnTopHint));

    drawNext();
}

GLOutput::~GLOutput() {
    mutex.lock();
    delete frame;
    mutex.unlock();
}

void GLOutput::setFrame(VideoFrame * frame) {
    mutex.lock();
    delete this -> frame;
    this -> frame = frame;
    mutex.unlock();
}

void GLOutput::drawNext() {
    if (frame -> image == 0) {
        close();
        return;
    }

    repaint();
    timer.singleShot(frame -> interval, this, SLOT(drawNext()));
}

void GLOutput::paintEvent(QPaintEvent*) {
    QPainter p(this);

    //Set the painter to use a smooth scaling algorithm.
    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
    mutex.lock();
    p.drawImage(this -> rect(), *frame -> image);
    mutex.unlock();
}

bool GLOutput::event(QEvent * event) {
    if (event -> type() == QEvent::MouseButtonDblClick) {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);

        if (mouseEvent -> button() == Qt::LeftButton) {
           if (isFullScreen())
              showNormal();
           else
              showFullScreen();
        }
    }

    if (event -> type() == QEvent::KeyPress) {
        QKeyEvent * keyEvent = static_cast <QKeyEvent *> (event);

        if (keyEvent -> key() == Qt::Key_Escape && isFullScreen()) {
           showNormal();
        }
    }

    return QGLWidget::event(event);
}
