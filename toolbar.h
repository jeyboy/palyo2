#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QMimeData>
#include <QMouseEvent>

#include "dnd.h"

class ToolBar : public QToolBar {
    Q_OBJECT

public:
    ToolBar(const QString &title, QWidget *parent = 0);
    ~ToolBar();

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    bool event(QEvent * ev) {
        emit(eventTriggered(ev));
        return QToolBar::event(ev);
    }

signals:
    void eventTriggered(QEvent * ev);
};

#endif // TOOLBAR_H
