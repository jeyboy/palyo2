#include "soundcloud_file.h"
#include "web/socials/soundcloud_api.h"
#include <QDebug>


SoundcloudFile::SoundcloudFile(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
    uid = hash -> value("u").toString();
    owner_uid = hash -> value("o").toString();
}

SoundcloudFile::SoundcloudFile(QString fileName, QString fileExtension, QString ownerID, QString fileID, ModelItem *parent, int genre_id, QString itemDuration, int itemSize, QString itemInfo, int initState)
    : ModelItem("", fileName, parent, genre_id, itemDuration, itemSize, itemInfo, initState) {

    uid = fileID;
    owner_uid = ownerID;
    extension = fileExtension;
}

SoundcloudFile::~SoundcloudFile() {}

bool SoundcloudFile::removePhysicalObject() {
    //TODO: realization require
    return false;
//    return QFile::remove(fullPath());
}

bool SoundcloudFile::isExist() const {
    //TODO: realization require
    return true;
//    return QFile::exists(fullPath());
}

bool SoundcloudFile::isRemote() const { return true; }

QString SoundcloudFile::toUID() {
    return buildUid(owner_uid, uid);
}

QUrl SoundcloudFile::toUrl() {
    return QUrl(path);
}

QJsonObject SoundcloudFile::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = SOUNDCLOUD_FILE;
    root["u"] = uid;
    root["o"] = owner_uid;

    return root;
}
