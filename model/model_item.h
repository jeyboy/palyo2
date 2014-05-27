#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

/// JSON names
/// a - item info
/// b - item bytes size
/// c - children
/// d - duration
/// e - extension
/// g - genreID
/// i - item type
/// l - tab items count
/// n - tab name
/// p - path
/// s - state
/// set - tab settings
/// o - web item owner id
/// t - title
/// u - web item id
/// vk - vk settings
///

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>

#include "model_item_state.h"

#include "misc/settings.h"

#define TITLEID 0
#define EXTENSIONID 1
#define PATHID 2
#define FOLDERID 3
#define STATEID 4
#define TITLESCACHEID 5
#define PROGRESSID 101
#define INFOID 102
#define ADDFONTID 103

#define FILE_ITEM 100
#define FOLDER_ITEM 200
#define CUE_ITEM 300
#define VK_FOLDER 400
#define VK_FILE 500

//TODO: add list of extensions for extension serialization to extension index in list

class ModelItem {
public:   
    ModelItem(int initState = STATE_DEFAULT | STATE_CHECKED);
    ModelItem(QJsonObject * hash, ModelItem * parent = 0);
    ModelItem(const QString filePath, QString fileName, ModelItem * parent = 0, int genre_id = -1, QString itemDuration = "", int itemSize = -1, QString itemInfo = "", int initState = STATE_DEFAULT | STATE_CHECKED);
    virtual ~ModelItem();

    QString fullPath() const;
    QString getDownloadTitle() const;
    QString getTitle() const;
    void openLocation();
    virtual bool removePhysicalObject() const = 0;

    virtual bool isExist() const = 0;
    virtual bool isRemote() const;
    virtual bool isFolder() const;
    bool isPlayable() const;

    bool hasInfo() const;
    void setInfo(QString newInfo);

    void setDuration(QString newDuration);
    void setGenre(int newGenreID);

    int getDownloadProgress() const;
    void setDownloadProgress(int percentageVal);

    QStringList getInfo() const;

    virtual QUrl toUrl();
    virtual QJsonObject toJSON();
    ModelItem * toModelItem();
    QString toPath() const;

    ModelItem * parent();

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

    ModelItemState *getState() const;
    void setState(int new_state, bool append_to_library = true);

    virtual void dropExpandProceedFlags();  // stub
    virtual void changeCheckedState(bool checked);  // stub

    bool cacheIsPrepared() const;
    void setCache(QList<QString> *newCache);
    void addToCache(QString title);
    QList<QString> * getTitlesCache() const;

    virtual QHash<QString, ModelItem *> * foldersList() const; // stub
    virtual int removeFolder(QString name);  // stub

    virtual QList<ModelItem *> * childItemsList(); // stub
protected:

    qint8 progress;
    QList<QString> * titlesCache;

    ModelItem * parentItem;
    ModelItemState * state;

    QString path;
    QString title;
    QString extension;
    QString info;

    int size;
    QString duration;
    qint16 genreID;
};


#endif // MODEL_ITEM_H
