#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

//#include <QFile>
//#include <QDir>
//#include <QVariant>
//#include <QAbstractItemModel>
//#include <QJsonObject>
//#include <QJsonArray>

#include "model_item.h"
#include "file_item.h"

class FolderItem : public ModelItem {
public:
    FolderItem();
    FolderItem(QJsonObject * hash, ModelItem *parent = 0);
    FolderItem(const QString filePath, ModelItem *parent = 0, int initState = STATE_DEFAULT);
    ~FolderItem();

    void openLocation();
    bool removePhysicalObject() const;

    bool isExist() const;
    bool isFolder() const;

    QJsonObject toJSON();

    ModelItem *child(int row);
    int childTreeCount() const;
    int childCount() const;
    void insertChild(int pos, ModelItem *item);
    void appendChild(ModelItem *child);
    bool removeChildren(int position, int count);

    void dropExpandProceedFlags();

    QHash<QString, ModelItem *> * foldersList() const;
    int removeFolder(QString name);
protected:
    QList<ModelItem *> * childItemsList();
private:
    QHash<QString, ModelItem *> *folders;
    QList<ModelItem *> childItems;
};


#endif // FOLDER_ITEM_H
