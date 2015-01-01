#include "output_container.h"
#include "media/media_player/media_player.h"

OutputContainer::OutputContainer(MasterClock * clock, QWidget * parent)
    : QWidget(parent)
    , clock(clock)
    , activated(false) {
    setMouseTracking(true);
    panel = new ControlPanel(clock, this);
}

bool OutputContainer::event(QEvent * event) {
    if (event -> type() == QEvent::WindowActivate/*ApplicationActivate*/) {
        activated = true;
    }

    if (event -> type() == QEvent::MouseButtonRelease) {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);
        MediaPlayer * player = (MediaPlayer * )clock -> mediaPlayer();

        if (mouseEvent -> button() == Qt::LeftButton) {
            if (activated) {
                activated = false;
            } else {
                if (player -> isPlayed())
                    player -> pause();
                else player -> play();
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
