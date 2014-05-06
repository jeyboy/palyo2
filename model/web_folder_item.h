#ifndef WEB_FOLDER_ITEM_H
#define WEB_FOLDER_ITEM_H

#include "folder_item.h"

class WebFolderItem : public FolderItem {
public:
    WebFolderItem();
    WebFolderItem(QJsonObject * hash, ModelItem *parent = 0);
    WebFolderItem(const QString filePath, ModelItem *parent = 0, int initState = STATE_UNPROCESSED);
    ~WebFolderItem();

    void openLocation();
    bool removePhysicalObject() const;

    QJsonObject toJSON();

    void insertChild(int pos, ModelItem *item);
    void appendChild(ModelItem *child);
    bool removeChildren(int position, int count);

protected:
    int aid;
};


#endif // FOLDER_ITEM_H
