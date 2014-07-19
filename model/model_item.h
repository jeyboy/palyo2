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
/// m - beat per minute
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

#define TITLEID Qt::UserRole + 1
#define EXTENSIONID Qt::UserRole + 2
#define PATHID Qt::UserRole + 3
#define FOLDERID Qt::UserRole + 4
#define STATEID Qt::UserRole + 5
#define TITLESCACHEID Qt::UserRole + 6
#define PROGRESSID Qt::UserRole + 7
#define INFOID Qt::UserRole + 8
#define ADDFONTID Qt::UserRole + 9

#define FILE_ITEM 100
#define FOLDER_ITEM 200
#define CUE_ITEM 300
#define VK_FOLDER 400
#define VK_FILE 500
#define SOUNDCLOUD_PLAYLIST 600
#define SOUNDCLOUD_FILE 700

//TODO: add list of extensions for extension serialization to extension index in list

class ModelItem {
public:   
    inline static QString buildUid(QString owner, QString id) { return owner + "_" + id;}

    ModelItem(int initState = STATE_DEFAULT | STATE_CHECKED);
    ModelItem(QJsonObject * hash, ModelItem * parent = 0);
    ModelItem(const QString filePath, QString fileName, ModelItem * parent = 0, int genre_id = -1, QString itemDuration = QString(), int itemSize = -1, QString itemInfo = QString(), int initState = STATE_DEFAULT | STATE_CHECKED);
    virtual ~ModelItem();

    QString fullPath() const;
    QString getDownloadTitle() const;
    inline QString getTitle() const { return title; }
    void openLocation();
    virtual bool removePhysicalObject() = 0;

    virtual bool isExist() const = 0;
    inline virtual bool isRemote() const { return false; }
    inline virtual bool isFolder() const { return false; }
    inline bool isPlayable() const {
        bool showBatch = Settings::instance() -> isCheckboxShow();
        return !isFolder() && (!showBatch || (showBatch && getState() -> isChecked()));
    }

    inline bool hasInfo() const {return !Settings::instance() -> isShowInfo() || (Settings::instance() -> isShowInfo() && !info.isEmpty());}
    inline void setInfo(QString newInfo) {info = newInfo;}

    inline void setBpm(int newBeat) { bpm = newBeat; }
    inline void setDuration(QString newDuration) {duration = newDuration;}
    inline void setGenre(int newGenreID) {genreID = newGenreID;}
    inline void setPath(QString newPath) {path = newPath;}

    QStringList getInfo() const;

    void accumulateUids(QHash<ModelItem*, QString> & store);

    virtual QUrl toUrl();
    virtual QJsonObject toJSON();
    ModelItem * toModelItem();
    QString toPath() const;
    virtual QString toUID() { return ""; }

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

    void shuffle();
protected:

    QList<QString> * titlesCache;

    ModelItem * parentItem;
    ModelItemState * state;

    QString path;
    QString title;
    QString extension;

    QString info;
    QString duration;

    qint16 bpm;
    qint16 genreID;

    int size;
};


#endif // MODEL_ITEM_H
