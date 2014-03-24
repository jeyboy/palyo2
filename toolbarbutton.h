#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QToolButton>
#include <QMimeData>
#include <QMouseEvent>
#include <QFile>

#include "dnd.h"
#include "itemlist.h"

class ToolbarButton : public QToolButton {
    Q_OBJECT

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public:
    void copyFile(QString path);
    ToolbarButton(QString text, QString folderPath, QWidget * parent=0);

    QString path;
};

#endif // TOOLBARBUTTON_H
