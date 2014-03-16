#include "toolbarbutton.h"
#include <QDebug>

ToolbarButton::ToolbarButton(QString text, QString folderPath, QWidget * parent) : QToolButton(parent) {
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
   if (event->mimeData()->hasFormat(DnD::instance() -> listItems)
           || event->mimeData()->hasFormat(DnD::instance() -> files)){
       event->accept();
   } else {
        event->ignore();
   }
}

void ToolbarButton::dropEvent(QDropEvent *event) {
      if (event->mimeData()->hasFormat(DnD::instance() -> listItems)) {

          event->accept();
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
