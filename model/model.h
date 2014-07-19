#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QFont>
#include <QBrush>
#include <QMimeData>
#include <QUrl>
#include <QFutureWatcher>

#include "misc/icon_provider.h"
#include "misc/settings.h"
#include "web/web_api.h"

#include "model_item.h"
#include "file_item.h"
#include "folder_item.h"
#include "model/web/vk/vk_folder.h"
#include "model/web/vk/vk_file.h"
#include "model/web/soundcloud/soundcloud_playlist.h"
#include "model/web/soundcloud/soundcloud_file.h"

class ModelItem;

class Model : public QAbstractItemModel {
    Q_OBJECT

public:
    Model(QJsonObject *hash = 0, QObject *parent = 0);
    ~Model();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(ModelItem * item) const;

    int itemsCount() const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void appendRow(ModelItem * item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    void clearAll(bool emitChanges = false);

    ModelItem *getItem(const QModelIndex &index) const;
//    template<class T> T *getItem(const QModelIndex &index) const;
    ModelItem * root() const;

    ModelItem * buildPath(QString path);
    bool isFolderExist(QString folderName, ModelItem * parent);
    ModelItem * addFolder(QString folderPath, QString folderName, ModelItem * parent, QString remoteID = "");
    ModelItem * addFolder(QString folder_name, ModelItem * parent, QString remoteID = "");

    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData * mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual QModelIndex dropProcession(const QList<QUrl> & list) = 0;

    void refreshItem(ModelItem * item);
    void removeFolderPrebuild(ModelItem * temp);

    virtual WebApi * getApi() { return 0; } //stub
signals:
    void itemsCountChanged(int newCount);
    void expandNeeded(const QModelIndex &index) const;
    void spoilNeeded(const QModelIndex &index) const;
    void showMessage(QString);

    void showSpinner();
    void hideSpinner();

    void updated();

public slots:
    void libraryResponse();
    virtual void refresh();
    void expanded(const QModelIndex &index);
    void collapsed(const QModelIndex &index);

protected:   
    virtual ModelItem * createItem(QString path, ModelItem * parent);
    QString folderName(QFileInfo & info);
    QFileInfoList folderFiles(QFileInfo file);
    QFileInfoList folderDirectories(QFileInfo file);

    int count;
    ModelItem * rootItem;
    QStringList filtersList;
};

#endif // MODEL_H
