#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QMimeData>
#include <QTreeView>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QJsonObject>

#include "model.h"
#include "model_item.h"
#include "model_item_delegate.h"
#include "dnd.h"

class TreeModel;
class ModelItem;
class ItemList;

class ItemList : public QTreeView {
  Q_OBJECT
public:
    ItemList(QWidget * parent, QJsonObject * attrs = 0);
    ~ItemList();

    QJsonObject toJSON();

    ModelItem * nextItem(QModelIndex currIndex);
    ModelItem * nextItem(ModelItem * curr);
    ModelItem * prevItem(ModelItem * curr);

    void updateTabCounter(QWidget * parentTab = 0, QTabWidget * container = 0);
    TreeModel * model;
protected:

//    void setModel(QAbstractItemModel *model);

//    void mousePressEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

    void startDrag(Qt::DropActions supportedActions);

    void filesRoutine(ModelItem * index, QFileInfoList list);
    void filesRoutine(ModelItem * index, QList<QUrl> list);

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_click(const QModelIndex &index);
    void on_doubleClick(const QModelIndex &index);
private:

//  QPixmap currentPixmap;
//  QPoint dragPoint;
};

//def __init__(self, parent=None):
//    super(FileList, self).__init__(parent)
//    self.setDragEnabled(True)
//    self.setAcceptDrops(True)
//    self.setDropIndicatorShown(True)
//    self.setSelectionMode(QAbstractItemView.SingleSelection)
//    #to allow internal reordering
//    self.setDefaultDropAction(Qt.MoveAction)

//def dropMimeData(self, index, mimeData, action):
//    if mimeData.hasUrls():
//        for url in mimeData.urls():
//            file = QFileInfo(url.toLocalFile())
//            item = QListWidgetItem(file.filePath())
//            self.insertItem(index, item)
//    return True

//def mimeTypes(self):
//    return ["text/uri-list"]





//class FileList(QListWidget):
//    def __init__(self, parent=None):
//        super(FileList, self).__init__(parent)
//        self.setAcceptDrops(True)
//        self.setDragEnabled(True)
//        self.setSelectionMode(QAbstractItemView.SingleSelection)
//        self.setDropIndicatorShown(True)
//        self.setDragDropMode(QAbstractItemView.InternalMove)

//    def dragEnterEvent(self, event):
//        if event.mimeData().hasUrls():
//            event.acceptProposedAction()
//        else:
//            super(FileList,self).dragEnterEvent(event)

//    def dragMoveEvent(self, event):
//        if event.mimeData().hasUrls():
//            event.acceptProposedAction()
//        else:
//            super(FileList, self).dragMoveEvent(event)

//    def dropEvent(self, event):
//        if event.mimeData().hasUrls():
//            urls = event.mimeData().urls()
//            if urls:
//                for url in urls:
//                    QListWidgetItem(url.toLocalFile(),self)
//            event.acceptProposedAction()
//        super(FileList, self).dropEvent(event)



#endif // ITEMLIST_H
