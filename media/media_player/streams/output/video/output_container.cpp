#include "output_container.h"


OutputContainer::OutputContainer(QWidget* parent) : QWidget(parent) {
}

bool OutputContainer::event(QEvent * event) {
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

    return QWidget::event(event);
}
