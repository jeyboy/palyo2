#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QMimeData>
#include <QTreeView>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QJsonObject>
#include <QDrag>
#include <QHeaderView>

#include "player.h"
#include "model.h"
#include "model_item.h"
#include "model_item_delegate.h"

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

    Model * getModel() const;

    CBHash getSettings() const;
    void setSettings(CBHash newSettings);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);

    void markSelectedAsLiked();

    void execItem(ModelItem * item);
    void removeItem(ModelItem * item);

protected:
    ModelItem * activeItem(bool next = true);
    ModelItem * nextItem(QModelIndex currIndex);
    ModelItem * nextItem(ModelItem * curr);
    ModelItem * prevItem(ModelItem * curr);

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
    Model * model;
    CBHash settings;
    QPoint dragStartPoint;
};

#endif // ITEMLIST_H
