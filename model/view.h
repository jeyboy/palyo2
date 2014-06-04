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
#include <QMenu>
#include <QMessageBox>
#include <QList>

#include "dialogs/tabdialog.h"

#include "media/player.h"
#include "model.h"
#include "model_item_delegate.h"

#include "model_item.h"

#include "misc/settings.h"

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, int> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH


class Model;
//TODO: move filters in separate singleton

class View : public QTreeView {
  Q_OBJECT
public:
    View(Model * newModel, QWidget * parent, CBHash settinsSet);
    ~View();

    virtual QJsonObject toJSON();

    void scrollToActive();

    void proceedPrev();
    void proceedNext();
    void deleteCurrentProceedNext();

    bool isRemoveFileWithItem();
    bool isPlaylist();
    bool isCommon();
    bool isEditable();

//    template<class T> T * getModel() const;
    Model * getModel() const;

    CBHash getSettings() const;
    void setSettings(CBHash newSettings);

    QModelIndexList selectedItems() const;

    bool execItem(ModelItem * item, bool paused = false);
    void removeItem(ModelItem * item);

    int itemsCount() const;

    virtual QModelIndex dropProcession(const QList<QUrl> & list) = 0;

    void downloadSelected(QString savePath, bool markAsLiked = false);
    void copyItemsFrom(View * otherView);

signals:
    void showSpinner();
    void hideSpinner();

public slots:
    void shuffle();
    void updateSelection(QModelIndex candidate);
    void startRoutine();
    void stopRoutine();
    void setHeaderText(QString);

protected slots:
    void onDoubleClick(const QModelIndex &index);
    void showContextMenu(const QPoint &);
    void openLocation();

    void download();
    void downloadAll();
    void modelUpdate();

protected:
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void resizeEvent(QResizeEvent *);
    bool prepareDownloading(QString path);

    void downloadItem(ModelItem * item, QString savePath);
    void downloadBranch(ModelItem * rootNode, QString savePath);


    QModelIndex activeItem(bool next = true);
    QModelIndex nextItem(QModelIndex & curr);
    QModelIndex prevItem(QModelIndex & curr);

    QFileInfoList folderFiles(QFileInfo file);
    QFileInfoList folderDirectories(QFileInfo file);

    virtual ModelItem * createItem(QString path, ModelItem * parent);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);

    Model * model;
    CBHash settings;
    QStringList filtersList;
    QPoint dragStartPoint;
};

#endif // VIEW_H
