#include "web_file_item.h"
#include <QDebug>


WebFileItem::WebFileItem(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
    uid = hash -> value("u").toString();
    duration = hash -> value("d").toInt();

    if (parent != 0) {
       parent -> appendChild(this);
    }
}

WebFileItem::WebFileItem(QString filePath, QString fileName, QString fileID, ModelItem *parent, int genre_id, int itemDuration, int initState)
    : ModelItem(filePath, fileName, parent, genre_id, initState) {

    uid = fileID;
    duration = itemDuration;
    extension = "mp3";

    if (parent != 0) {
       parent -> insertChild(0, this);
    }
}

WebFileItem::~WebFileItem() {}

bool WebFileItem::removePhysicalObject() const {
    //TODO: realization require
    return false;
//    return QFile::remove(fullPath());
}

bool WebFileItem::isExist() const {
    //TODO: realization require
    return true;
//    return QFile::exists(fullPath());
}

bool WebFileItem::isRemote() const { return true; }

QUrl WebFileItem::toUrl() {
    return QUrl(path);
}

QJsonObject WebFileItem::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = WEB_FILE_ITEM;
    root["u"] = uid;
    root["d"] = duration;

    return root;
}
