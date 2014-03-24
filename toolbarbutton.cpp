#include "toolbarbutton.h"
#include <QDebug>

ToolbarButton::ToolbarButton(QString text, QString folderPath, QWidget * parent) : QToolButton(parent) {
    path = folderPath;

    if (!path.endsWith('/'))
        path = path.append('/') ;

    setText(text);
    setStyleSheet(
                "QToolButton {"
                    "border: 1px solid #444;"
                    "border-radius: 4px;"
                    "font-weight: bold;"
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
   if (event->mimeData()->hasFormat(DnD::instance() -> listItems)
           || event->mimeData()->hasFormat(DnD::instance() -> files)){
       event->accept();
   } else {
        event->ignore();
   }
}

void ToolbarButton::dropEvent(QDropEvent *event) {
      if (event -> mimeData() -> hasFormat(DnD::instance() -> listItems)) {
          QByteArray data = event -> mimeData() -> data(DnD::instance() -> listItems);
          QStringList list = QString(data).split('\n');

          foreach(QString str, list){
              copyFile(str);
          }

          ((ItemList *)event -> source()) -> markSelectedAsLiked();
          event -> accept();
      } else if (event->mimeData()->hasFormat(DnD::instance() -> files)) {
          if(event -> mimeData() -> hasUrls()) {
              QList<QUrl> list = event -> mimeData() -> urls();
              foreach(QUrl url, list) {
                  qDebug() << url;
              }
          }

          event->accept();
      } else {  event->ignore(); }
}

void ToolbarButton::copyFile(QString copyPath) {
    QString prepared_path = path + copyPath.section('/', -1, -1);

    if (QFile::exists(prepared_path)) {
        QFile::remove(prepared_path);
    }

    QFile::copy(path, prepared_path);
}
