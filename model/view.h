#ifndef VIEW_H
#define VIEW_H

#include <QMimeData>
#include <QTreeView>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QJsonObject>
#include <QDrag>
#include <QHeaderView>
#include <QDesktopServices>
#include <QMenu>
#include <QMessageBox>

#include "media/player.h"
#include "model.h"
#include "model_item.h"
#include "model_item_delegate.h"

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, bool> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

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

    Model * getModel() const;

    CBHash getSettings() const;
    void setSettings(CBHash newSettings);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);

    void markSelectedAsLiked();

    bool execItem(ModelItem * item);
    void removeItem(ModelItem * item);

public slots:
//    void changeSelection(const QModelIndex & index);
    void updateSelection(QModelIndex candidate);

private slots:
    void on_doubleClick(const QModelIndex &index);
    void showContextMenu(const QPoint &);
    void openLocation();

protected:
    ModelItem * activeItem(bool next = true);
//    ModelItem * nextItem(QModelIndex currIndex);
    ModelItem * nextItem(ModelItem * curr);
    ModelItem * prevItem(ModelItem * curr);

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

    void filesRoutine(ModelItem * index, QFileInfoList list);
    void filesRoutine(ModelItem * index, QList<QUrl> list);

    void keyPressEvent(QKeyEvent *event);

private:
    Model * model;
    CBHash settings;
    QPoint dragStartPoint;
};

#endif // VIEW_H
