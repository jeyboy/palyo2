/// JSON names
/// t - title
/// s - state
/// p - path
/// e - extension
/// c - children
/// i - item type
/// l - tab items count
/// n - tab name
/// set - tab settings
///


#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>

#include "model_item_state.h"
#include "media/library.h"

#define TITLEID 0
#define EXTENSIONID 1
#define PATHID 2
#define FOLDERID 3
#define STATEID 4

#define FILE_ITEM 0
#define FOLDER_ITEM 1
#define CUE_ITEM 2
#define WEB_ITEM 3

//TODO: add list of extensions for extension serialization to extension index in list

class ModelItem {
public:   
    ModelItem();
    ModelItem(QJsonObject * hash, ModelItem *parent = 0);
    ModelItem(const QString filepath, ModelItem *parent = 0, int init_state = STATE_DEFAULT);
    ~ModelItem();

    QString fullPath() const;
    QString getTitle() const;
    void openLocation();

    bool isExist() const;
    bool isFolder() const;

    QUrl toUrl();
    QJsonObject toJSON();

    ModelItem *parent();

    ModelItem *child(int row);
    int childTreeCount() const;
    int childCount() const;
    void insertChild(int pos, ModelItem *item);
    void appendChild(ModelItem *child);
    bool removeChildren(int position, int count);

    int column() const;
    int columnCount() const;

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    ModelItemState * getState() const;
    void setState(int new_state, bool append_to_library = true);

    void dropExpandProceedFlags();

    QHash<QString, ModelItem *> * foldersList() const;
    int removeFolder(QString name);

    bool cacheIsPrepared() const;
    void setCache(QList<QString> * newCache);
    void addToCache(QString title);
    QList<QString> * getTitlesCache() const;
private:
    void init(bool isFolder);
    void rootItemInit();

    QHash<QString, ModelItem *> * folders;
    QList<QString> * titlesCache;

    QList<ModelItem*> childItems;
    ModelItem *parentItem;  

    QString title;
    QString path;
    QString extension;
    ModelItemState * state;
};


#endif // MODEL_ITEM_H
