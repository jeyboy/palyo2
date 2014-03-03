#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QToolButton>
#include <QMimeData>
#include <QMouseEvent>

class ToolbarButton : public QToolButton {
    Q_OBJECT

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public:
    ToolbarButton(QString text, QString folderPath, QWidget * parent=0);

    QString path;

private:
    QString system;
    QString inner;
};

#endif // TOOLBARBUTTON_H
