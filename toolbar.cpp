#include "toolbar.h"
#include <qDebug>

ToolBar::ToolBar(const QString &title, QWidget * parent) : QToolBar(title, parent) {

}

ToolBar::~ToolBar() {

}

void ToolBar::dragEnterEvent(QDragEnterEvent *event) {
   if (event->mimeData()->hasFormat(DnD::instance() -> files)){
       event->accept();
   } else {
        event->ignore();
   }
}

void ToolBar::dropEvent(QDropEvent *event) {
  if (event->mimeData()->hasFormat(DnD::instance() -> files)) {
      if(event -> mimeData() -> hasUrls()) {
          QList<QUrl> list = event -> mimeData() -> urls();
          foreach(QUrl url, list) {
              qDebug() << url;
          }
      }

      event->accept();
  } else {  event->ignore(); }
}
