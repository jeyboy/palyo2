#include "web_folder_item.h"
#include <QDebug>

///////////////////////////////////////////////////////////
WebFolderItem::WebFolderItem() : FolderItem(STATE_UNPROCESSED) {
    folders = new QHash<QString, ModelItem *>();
}

WebFolderItem::WebFolderItem(QJsonObject * hash, ModelItem *parent) : FolderItem(hash, parent) {
    folders = new QHash<QString, ModelItem *>();

    if (parent != 0) {
        parent -> foldersList() -> insert(title, this -> toModelItem());
        parent -> appendChild(this -> toModelItem());
    }

    if (hash -> contains("c")) {
        QJsonArray ar = hash -> value("c").toArray();
        QJsonObject iterObj;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            switch(iterObj.value("i").toInt()) {
                case FILE_ITEM: {
                    new FileItem(&iterObj, this -> toModelItem());
                break;}
                case FOLDER_ITEM: {
                    new FolderItem(&iterObj, this -> toModelItem());
                break;}
                // case CUE_ITEM: {
                // new CueItem(&iter_obj, this -> toModelItem());
                // break;}
                // case WEB_ITEM: {
                // new WebItem(&iter_obj, this -> toModelItem());
                // break;}
            }
        }
    }
}

WebFolderItem::WebFolderItem(QString filePath, QString folderTitle, ModelItem *parent, int initState) : FolderItem(filePath, folderTitle, parent, initState) {
    folders = new QHash<QString, ModelItem *>();
    title = filePath;

    if (parent != 0) {
        parent -> foldersList() -> insert(title, this -> toModelItem());
        parent -> appendChild(this -> toModelItem());
    }
}

WebFolderItem::~WebFolderItem() {
    qDeleteAll(childItems);

    delete folders;
}

void WebFolderItem::openLocation() {
    QDesktopServices::openUrl(toUrl());
}

bool WebFolderItem::removePhysicalObject() const {
    QDir delDir(fullPath());
    if (fullPath().split('/').length() >= 2) {
        return delDir.removeRecursively();
    }

    return false;
}

bool WebFolderItem::isExist() const {
    return QDir(fullPath()).exists();
}

//bool WebFolderItem::isFolder() const {
//    return true;
//}

//QJsonObject WebFolderItem::toJSON() {
//    QJsonObject root = ModelItem::toJSON();

//    root["i"] = FOLDER_ITEM;

//    if (childItems.length() > 0) {
//        QJsonArray ar = QJsonArray();
//        for(int i=0; i < childItems.length(); i++)
//            ar.append(childItems.at(i) -> toJSON());

//        root["c"] = ar;
//    }

//    return root;
//}


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
