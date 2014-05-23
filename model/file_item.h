#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <QFile>
#include <QJsonObject>

#include "model_item.h"

class FileItem : public ModelItem {
public:
    FileItem(QJsonObject *hash, ModelItem *parent = 0);
    FileItem(const QString filepath, ModelItem *parent = 0, int genre_id = -1, int itemDuration = -1, int init_state = STATE_DEFAULT | STATE_CHECKED);
    ~FileItem();

    bool removePhysicalObject() const;

    bool isExist() const;

    QJsonObject toJSON();
protected:
    QString fileExtension(QString filePath);
    QString fileTitle(QString filePath);
};


#endif // FILE_ITEM_H
