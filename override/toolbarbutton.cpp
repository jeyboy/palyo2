#include "toolbarbutton.h"
#include <QDebug>

ToolbarButton::ToolbarButton(QString text, QString folderPath, QWidget * parent) : QToolButton(parent) {
    path = folderPath;

    if (!path.endsWith('/'))
        path = path.append('/') ;

    QFile::setPermissions(path, QFile::WriteUser | QFile::ReadUser);

    setText(text);
    setStyleSheet(
                "QToolButton {"
                    "border: 1px solid #444;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #FFFFFF, stop:0.8 #E7DA1E, stop:1 #F7E488);"
//                  "background: qradialgradient(cx:0.5, cy:0.5, radius: 1, fx:0.2, fy:0.2, stop:0 #FFFFFF, stop:0.5 #B3AF76, stop:1 #F7E488);"
                    "border-radius: 8px;"
                    "font-weight: bold;"
                    "height: 24px;"
                    "min-width: 24px;"
                    "margin: 0 2px;"
                "}"
                "QToolButton:hover {"
                    "color: #FFF;"
                    "background-color: #000;"
                "}"
                );

    setAcceptDrops(true);
    /*setDefaultDropAction(Qt::MoveAction);*/
}

void ToolbarButton::dragEnterEvent(QDragEnterEvent *event) {
    if (event -> mimeData() -> hasFormat("text/uri-list")) {
        event -> accept();
    } else {
        event -> ignore();
    }
}

void ToolbarButton::dropEvent(QDropEvent *event) {
    if (event -> mimeData() -> hasUrls()) {
        View * view = (View *)event -> source();

        if (!QString(view -> metaObject() -> className()).endsWith("View"))
            qDebug() << "Out request";

        view -> downloadSelected(path, true);

        event -> accept();
    } else { event -> ignore(); }
}
