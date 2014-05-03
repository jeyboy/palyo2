#include "model_item.h"
#include <QDebug>

/////////////////////////////////////////////////////////

ModelItem::ModelItem(int initState) {
    parentItem = 0;
    titlesCache = 0;
    state = new ModelItemState(initState);
    path = QString();
    title = QString("--(O_o)--");
    extension = QString();
}

ModelItem::ModelItem(QJsonObject *hash, ModelItem *parent) {
    parentItem = parent;
    titlesCache = 0;
    state = new ModelItemState(hash -> value("s").toInt());
    path = hash -> value("p").toString();
    title = hash -> value("t").toString();
    extension = hash -> value("e").toString();
}

ModelItem::ModelItem(QString filePath, ModelItem *parent, int initState) {
    parentItem = parent;
    titlesCache = 0;
    state = new ModelItemState(initState);

    initInfo(filePath);
}

ModelItem::~ModelItem() {
    delete state;
    delete titlesCache;
}


QString ModelItem::fullPath() const {
    if (parentItem != 0) {
        return parentItem -> fullPath() + '/' + path;
    }

    return "";


//    ModelItem * curr = parentItem;
//    QString path_buff = "";

//    while(curr != 0) {
//        path_buff = curr -> path + '/' + path_buff;
//        curr = curr -> parentItem;
//    }

//    if (extension.isEmpty())
//        return path_buff.mid(1) + title;
//    else
//        return path_buff.mid(1) + title + '.' + extension;
}

QString ModelItem::getTitle() const {
    return title;
}

void ModelItem::openLocation() {
    QDesktopServices::openUrl(toUrl());
}

bool ModelItem::isFolder() const {
    return false;
}


QUrl ModelItem::toUrl() {
    return QUrl::fromLocalFile(fullPath());
}

QJsonObject ModelItem::toJSON() {
    QJsonObject root = QJsonObject();

    root["t"] = title;
    root["s"] = state -> getFuncValue();
    root["p"] = path;

    if (!extension.isNull())
        root["e"] = extension;

    return root;
}

ModelItem * ModelItem::toModelItem() {
    return dynamic_cast<ModelItem *>(this);
}


ModelItem *ModelItem::parent() {
    return parentItem;
}

ModelItem *ModelItem::child(int) { return 0; }

int ModelItem::childTreeCount() const { return 0; }

int ModelItem::childCount() const { return 0; }

void ModelItem::insertChild(int, ModelItem *) { }

void ModelItem::appendChild(ModelItem *) {}

bool ModelItem::removeChildren(int, int) { return false; }


int ModelItem::column() const { return 0; }

int ModelItem::columnCount() const { return 1; }


int ModelItem::row() const {
     if (parentItem)
         return parentItem -> childItemsList() -> indexOf(const_cast<ModelItem *>(this));

     return 0;
}


QVariant ModelItem::data(int column) const {
    switch(column) {
        case TITLEID: return getTitle();
        case EXTENSIONID: return extension;
        case PATHID: return path;
        case FOLDERID: return isFolder();
        default: return QVariant();
    }
}

bool ModelItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= 2)
//    if (column < 0 || column >= itemData.size())
        return false;

//    itemData[column] = value;

    switch(column) {
        case TITLEID: title = value.toString();
        case EXTENSIONID: extension = value.toString();
        case PATHID: path = value.toString();
    }

    return true;
}


void ModelItem::proceedByLibrary(const QModelIndex & index) {
//    Library::instance() -> initItem(index);
}

ModelItemState *ModelItem::getState() const {
    return state;
}

void ModelItem::setState(int new_state, bool append_to_library) {
//    if (state -> setBit(new_state) && append_to_library) {
//        if (state -> isListened())
//            Library::instance() -> addItem(this, STATE_LISTENED);
//        else if (state -> isLiked())
//            Library::instance() -> addItem(this, STATE_LIKED);
//    }
}


void ModelItem::dropExpandProceedFlags() {}


QHash<QString, ModelItem *> *ModelItem::foldersList() const { return 0; }
int ModelItem::removeFolder(QString) { return -1; }


bool ModelItem::cacheIsPrepared() const {
    return titlesCache == 0;
}

void ModelItem::setCache(QList<QString> * newCache) {
    titlesCache = newCache;
}

void ModelItem::addToCache(QString title) {
    if (!titlesCache -> contains(title))
        titlesCache -> append(title);
}
QList<QString> *ModelItem::getTitlesCache() const {
    return titlesCache;
}

QList<ModelItem *> * ModelItem::childItemsList() { return 0;}

void ModelItem::initInfo(QString filePath) {
    title = path = filePath;
    extension = QString();
}
