#ifndef VIEW_H
#define VIEW_H

#define LIST_VIEW 0
#define TREE_VIEW 1
#define LEVEL_TREE_VIEW 2

#include <QMimeData>
#include <QTreeView>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QJsonObject>
#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>

#include "media/player.h"
#include "model.h"
#include "model_item.h"
#include "file_item.h"
#include "folder_item.h"
#include "model_item_delegate.h"

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, bool> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

//TODO: move filters in separate singleton

class View : public QTreeView {
  Q_OBJECT
public:
    View(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~View();

    QJsonObject toJSON();

    void proceedPrev();
    void proceedNext();
    void deleteCurrentProceedNext();

    bool isRemoveFileWithItem();
    bool isPlaylist();

//    template<class T> T * getModel() const;
    Model * getModel() const;

    CBHash getSettings() const;
    void setSettings(CBHash newSettings);

    void markSelectedAsLiked();

    bool execItem(ModelItem * item);
    void removeItem(ModelItem * item);

public slots:
    void updateSelection(QModelIndex candidate);

protected slots:
    void onDoubleClick(const QModelIndex &index);
    void showContextMenu(const QPoint &);
    void openLocation();

protected:
    virtual Model * newModel(QJsonObject * hash = 0) = 0;

    ModelItem * activeItem(bool next = true);
//    ModelItem * nextItem(QModelIndex currIndex);
    ModelItem * nextItem(ModelItem * curr);
    ModelItem * prevItem(ModelItem * curr);

    QFileInfoList folderEntities(QFileInfo file);
    virtual QModelIndex dropProcession(const QList<QUrl> & list) = 0;
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    virtual void filesRoutine(ModelItem * index, QFileInfoList list) = 0;
    virtual void filesRoutine(ModelItem * index, QList<QUrl> list) = 0;

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);

    Model * model;
    CBHash settings;
    QStringList filtersList;
    QPoint dragStartPoint;
};

#endif // VIEW_H
