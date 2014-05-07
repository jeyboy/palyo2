#include "file_item.h"
#include <QDebug>


FileItem::FileItem(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
    if (parent != 0) {
       parent -> appendChild(this);
    }
}

FileItem::FileItem(QString filePath, ModelItem *parent, int initState) : ModelItem(filePath, filePath.section('/', -1, -1), parent, initState) {
    extension = title.section('.', -1, -1);
    if (extension != title)
        title = title.section('.', 0, -2);
    else extension = QString();

    if (parent != 0) {
       parent -> insertChild(0, this);
    }
}

FileItem::~FileItem() {

}

bool FileItem::removePhysicalObject() const {
    return QFile::remove(fullPath());
}

bool FileItem::isExist() const {
    return QFile::exists(fullPath());
}

QJsonObject FileItem::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = FILE_ITEM;

    return root;
}
