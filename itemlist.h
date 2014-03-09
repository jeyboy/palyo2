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
#include "tab.h"
#include "dnd.h"

class TreeModel;
class ModelItem;
class ItemList;
class Tab;

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, bool> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

class ItemList : public QTreeView {
  Q_OBJECT
public:
    ItemList(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~ItemList();

    QJsonObject toJSON();

    void proceedPrev();
    void proceedNext();
    void deleteCurrentProceedNext();


    bool isRemoveFileWithItem();
    bool isPlaylist();

    TreeModel * getModel() const;
    void updateTabCounter(QWidget * parentTab = 0, QTabWidget * container = 0);

protected:
    ModelItem * activeItem(bool next = true);
    ModelItem * nextItem(QModelIndex currIndex);
    ModelItem * nextItem(ModelItem * curr);
    ModelItem * prevItem(ModelItem * curr);


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
    TreeModel * model;
    CBHash settings;

//  QPixmap currentPixmap;
//  QPoint dragPoint;
};

#endif // ITEMLIST_H
