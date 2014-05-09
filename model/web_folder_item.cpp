#include "web_folder_item.h"
#include <QDebug>

///////////////////////////////////////////////////////////
WebFolderItem::WebFolderItem() : FolderItem(STATE_UNPROCESSED) {
    folders = new QHash<QString, ModelItem *>();
}

WebFolderItem::WebFolderItem(QJsonObject * hash, ModelItem *parent) : FolderItem(hash, parent) {
    uid = hash -> value("u").toString();
}

WebFolderItem::WebFolderItem(QString filePath, QString folderID, QString folderTitle, ModelItem *parent, int initState) : FolderItem(filePath, folderTitle, parent, initState) {
    uid = folderID;
}

WebFolderItem::~WebFolderItem() {}

QUrl WebFolderItem::toUrl() {
    return QUrl(path);
}

void WebFolderItem::openLocation() {
    QDesktopServices::openUrl(toUrl());
}

bool WebFolderItem::removePhysicalObject() const {
    //TODO: add realization

    return false;
}

bool WebFolderItem::isExist() const {   
    //TODO: add realization

    return true;
}

QJsonObject WebFolderItem::toJSON() {
    QJsonObject root = FolderItem::toJSON();

    root["i"] = WEB_FOLDER_ITEM;
    root["u"] = uid;

    return root;
}


//QList<ModelItem *> * WebFolderItem::childItemsList() {
//    return &childItems;
//}

//ModelItem *WebFolderItem::child(int row) {
//    return childItems.value(row);
//}

//int WebFolderItem::childTreeCount() const {
//    int ret = childItems.count() - foldersList() -> count();
//    foreach(ModelItem * folder, foldersList() -> values()) {
//        ret += folder -> childTreeCount();
//    }


////    int ret = 0;
////    foreach(ModelItem * childItem, childItems) {
////        if (childItem -> folders == 0) // not is unprocessed
////            ret += 1;
////        else
////            ret += childItem -> childTreeCount();
////    }

//    return ret;
//}

//int WebFolderItem::childCount() const {
//    return childItems.count();
//}

//void WebFolderItem::insertChild(int pos, ModelItem *item) {
//    childItems.insert(pos, item);
//}

//void WebFolderItem::appendChild(ModelItem *item) {
//    childItems.append(item);
//}

//bool WebFolderItem::removeChildren(int position, int count) {
//    if (position < 0 || position + count > childItems.size())
//        return false;

//    for (int row = 0; row < count; ++row)
//        delete childItems.takeAt(position);

//    return true;
//}

//void WebFolderItem::dropExpandProceedFlags() {
//    getState() -> unsetProceed();
//    foreach(ModelItem *item, folders -> values()) {
//        item -> dropExpandProceedFlags();
//    }
//}


//QHash<QString, ModelItem *> * WebFolderItem::foldersList() const {
//    return folders;
//}
//int WebFolderItem::removeFolder(QString name) {
//    return folders -> remove(name);
//}
