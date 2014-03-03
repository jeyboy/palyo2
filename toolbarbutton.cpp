#include "toolbarbutton.h"
#include <qDebug>

ToolbarButton::ToolbarButton(QString text, QString folderPath, QWidget * parent) : QToolButton(parent) {
    system = "text/uri-list";
    inner = "application/x-QListView-DragAndDrop";

    path = folderPath;
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
   if (event->mimeData()->hasFormat(system)){
       event->accept();
   } else {
        event->ignore();
   }
}

void ToolbarButton::dropEvent(QDropEvent *event) {
      if (event->mimeData()->hasFormat(inner)) {

          event->accept();
      } else if (event->mimeData()->hasFormat(system)) {
          if(event -> mimeData() -> hasUrls()) {
              QList<QUrl> list = event -> mimeData() -> urls();
              foreach(QUrl url, list) {
                  qDebug() << url;
              }
          }

          event->accept();
      } else {  event->ignore(); }
}
