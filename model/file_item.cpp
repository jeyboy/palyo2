#include "file_item.h"
#include <QDebug>


FileItem::FileItem(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
    if (parent != 0) {
       parent -> insertChild(0, this);
    }


//    parentItem = parent;
//    init(hash -> contains("p"));
//    state = new ModelItemState(hash -> value("s").toInt());

//    if (hash -> contains("p")) {
//        title = path = hash -> value("p").toString();

//        if (parent != 0)
//            parent -> folders -> insert(title, this);
//    } else {
//        title = hash -> value("n").toString();
//        extension = hash -> value("e").toString();
//    }

//    if (hash -> contains("c")) {
//        QJsonArray ar = hash -> value("c").toArray();
//        QJsonObject iter_obj;

//        foreach(QJsonValue obj, ar) {
//            iter_obj = obj.toObject();
//            new ModelItem(&iter_obj, this);
//        }
//    }

//    if (parent != 0) {
//        parent -> appendChild(this);
//    } else {
//        rootItemInit();
//    }
}

FileItem::FileItem(QString filePath, ModelItem *parent, int initState) : ModelItem(filePath, parent, initState) {
    if (parent != 0) {
       parent -> insertChild(0, this);
    }

//    state = new ModelItemState(init_state);
//    parentItem = parent;

//    if (!state -> isUnprocessed()) {
//        init(false);
//        path = file_path.section('/', 0, -2);
//        title = file_path.section('/', -1, -1);
//        extension = title.section('.', -1, -1);
//        if (extension != title)
//            title = title.section('.', 0, -2);
//        else extension = QString();

//    } else {
//        init(true);
//        title = path = file_path;
//        extension = QString();

//        if (parent != 0)
//            parent -> folders -> insert(title, this);
//    }

//    if (parent != 0) {
//        if (!state -> isUnprocessed()) {
//            parent -> insertChild(0, this);
//        } else {
//            parent -> appendChild(this);
//        }
//    }
}

FileItem::~FileItem() {

}

QJsonObject FileItem::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = FILE_ITEM;

    return root;
}
