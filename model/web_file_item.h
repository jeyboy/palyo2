#ifndef WEB_FILE_ITEM_H
#define WEB_FILE_ITEM_H

#include <QFile>
#include <QJsonObject>

#include "model_item.h"

class WebFileItem : public ModelItem {
public:
    WebFileItem(QJsonObject *hash, ModelItem *parent = 0);
    WebFileItem(const QString filepath, QString filename, QString fileID, ModelItem *parent = 0, int genre_id = -1, int itemDuration = -1, int init_state = STATE_DEFAULT | STATE_CHECKED);
    ~WebFileItem();

    bool removePhysicalObject() const;

    bool isExist() const;
    bool isRemote() const;

    QUrl toUrl();
    QJsonObject toJSON();
protected:
    QString uid;
    int duration;

//    QString fileExtension(QString filePath);
//    QString fileTitle(QString filePath);
};


#endif // WEB_FILE_ITEM_H
