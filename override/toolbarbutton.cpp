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
                    "background-color: #F7E488;"
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
        QList<QUrl> list = event -> mimeData() -> urls();

        foreach(QUrl url, list) {
            copyFile(url.toLocalFile());
        }

        event->accept();

        if (QString(event -> source() -> metaObject() -> className()).endsWith("View"))
            ((View *)event -> source()) -> markSelectedAsLiked();
        else
            qDebug() << "Out request";

    } else {  event->ignore(); }
}

void ToolbarButton::copyFile(QString copyPath) {
    QString prepared_path = path + copyPath.section('/', -1, -1);

    if (QFile::exists(prepared_path)) {
        QFile::remove(prepared_path);
    }

    QFile f(copyPath);
    if (!f.copy(prepared_path))
        QMessageBox::warning(this, "Bla", f.errorString());

//    if (!QFile::copy(copyPath, prepared_path))
//        QMessageBox::warning(this, "Bla", "Permissions not enough");
}
