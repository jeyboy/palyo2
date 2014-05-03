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
//TODO: deleting object of polymorphic class type 'ModelItem' which has non-virtual destructor might cause undefined behaviour

class ModelItem {
public:   
    ModelItem();
    ModelItem(QJsonObject * hash, ModelItem *parent = 0);
    ModelItem(const QString filePath, ModelItem *parent = 0, int initState = STATE_DEFAULT);
    virtual ~ModelItem();

    QString fullPath() const;
    QString getTitle() const;
    void openLocation();
    virtual bool removePhysicalObject() const = 0;

    virtual bool isExist() const = 0;
    bool isFolder() const;

    QUrl toUrl();
    QJsonObject toJSON();
    ModelItem * toModelItem();

    ModelItem *parent();

    ModelItem *child(int row);  // stub
    int childTreeCount() const;  // stub
    int childCount() const;  // stub
    void insertChild(int pos, ModelItem *item);  // stub
    void appendChild(ModelItem *child);  // stub
    bool removeChildren(int position, int count);  // stub

    int column() const;
    int columnCount() const;

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    ModelItemState *getState() const;
    void setState(int new_state, bool append_to_library = true);

    void dropExpandProceedFlags();  // stub

    bool cacheIsPrepared() const;
    void setCache(QList<QString> *newCache);
    void addToCache(QString title);
    QList<QString> * getTitlesCache() const;

    QHash<QString, ModelItem *> * foldersList() const; // stub
    int removeFolder(QString name);  // stub

protected:
    QList<ModelItem *> * childItemsList(); // stub
    void initInfo(QString filePath);

    QList<QString> *titlesCache;

    ModelItem *parentItem;  
    ModelItemState * state;

    QString path;
    QString title;
    QString extension;
};


#endif // MODEL_ITEM_H
