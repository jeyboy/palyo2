#include "output_container.h"
#include "media/media_player/media_player.h"
#include <QDebug>

OutputContainer::OutputContainer(MasterClock * clock, QWidget * parent) : QWidget(parent) {
    panel = new ControlPanel((MediaPlayer * )clock -> player, this);
}

bool OutputContainer::event(QEvent * event) {
    if (event -> type() == QEvent::MouseButtonRelease) {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);

        if (mouseEvent -> button() == Qt::LeftButton) {
            if (MediaPlayer::instance() -> isPlayed()) {
                MediaPlayer::instance() -> pause();
//                panel -> update pause button
            }
            else {
                MediaPlayer::instance() -> play();
//                panel -> update play button
            }
        }
    }

    if (event -> type() == QEvent::MouseButtonDblClick) {
        QMouseEvent * mouseEvent2 = static_cast <QMouseEvent *> (event);

        if (mouseEvent2 -> button() == Qt::LeftButton) {
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

    return QWidget::event(event);
}
