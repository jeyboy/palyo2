#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

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

#include <QFile>
#include <QDir>
#include <QUrl>
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
#define TITLESCACHEID 5

#define FILE_ITEM 100
#define FOLDER_ITEM 200
#define CUE_ITEM 300
#define WEB_ITEM 400

//TODO: add list of extensions for extension serialization to extension index in list

class ModelItem {
public:   
    ModelItem(int initState = STATE_DEFAULT);
    ModelItem(QJsonObject * hash, ModelItem *parent = 0);
    ModelItem(const QString filePath, ModelItem *parent = 0, int initState = STATE_DEFAULT);
    virtual ~ModelItem();

    QString fullPath() const;
    QString getTitle() const;
    void openLocation();
    virtual bool removePhysicalObject() const = 0;

    virtual bool isExist() const = 0;
    virtual bool isFolder() const;

    QUrl toUrl();
    virtual QJsonObject toJSON();
    ModelItem * toModelItem();

    ModelItem *parent();

    virtual ModelItem *child(int row);  // stub
    virtual int childTreeCount() const;  // stub
    virtual int childCount() const;  // stub
    virtual void insertChild(int pos, ModelItem *item);  // stub
    virtual void appendChild(ModelItem *child);  // stub
    virtual bool removeChildren(int position, int count);  // stub

    int column() const;
    int columnCount() const;

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    void proceedByLibrary(const QModelIndex & index);
    ModelItemState *getState() const;
    void setState(int new_state, bool append_to_library = true);

    virtual void dropExpandProceedFlags();  // stub

    bool cacheIsPrepared() const;
    void setCache(QList<QString> *newCache);
    void addToCache(QString title);
    QList<QString> * getTitlesCache() const;

    virtual QHash<QString, ModelItem *> * foldersList() const; // stub
    virtual int removeFolder(QString name);  // stub

protected:
    virtual QList<ModelItem *> * childItemsList(); // stub

    QList<QString> *titlesCache;

    ModelItem *parentItem;  
    ModelItemState * state;

    QString path;
    QString title;
    QString extension;
};


#endif // MODEL_ITEM_H
