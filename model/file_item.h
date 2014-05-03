#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include "model_item.h"

class FileItem : public ModelItem {
public:
    FileItem(QJsonObject *hash, ModelItem *parent = 0);
    FileItem(const QString filepath, ModelItem *parent = 0, int init_state = STATE_DEFAULT);
    ~FileItem();

    bool removePhysicalObject() const;

    bool isExist() const;

    QJsonObject toJSON();
protected:
    void initInfo(QString filePath);
};


#endif // FILE_ITEM_H
