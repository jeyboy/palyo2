#include "model_item.h"
#include <QDebug>

/////////////////////////////////////////////////////////

void ModelItem::init(bool isFolder) {
    titlesCache = 0;
    childItems = QList<ModelItem*>();

    if (isFolder) {
        folders = new QHash<QString, ModelItem *>();
    } else {
        folders = 0;
    }
}

void ModelItem::rootItemInit() {
    path = QString();
    title = QString("--(O_o)--");
    extension = QString();
}

ModelItem::ModelItem() {
    init(true);
    state = new ModelItemState(STATE_UNPROCESSED);
    rootItemInit();
    parentItem = 0;
}

ModelItem::ModelItem(QJsonObject * attrs, ModelItem *parent) {
    parentItem = parent;
    init(attrs -> contains("p"));
    state = new ModelItemState(attrs -> value("s").toInt());

    if (attrs -> contains("p")) {
        title = path = attrs -> value("p").toString();

        if (parent != 0)
            parent -> folders -> insert(title, this);
    } else {
        title = attrs -> value("n").toString();
        extension = attrs -> value("e").toString();
    }

    if (attrs -> contains("c")) {
        QJsonArray ar = attrs -> value("c").toArray();
        QJsonObject iter_obj;

        foreach(QJsonValue obj, ar) {
            iter_obj = obj.toObject();
            new ModelItem(&iter_obj, this);
        }
    }

    if (parent != 0) {
        parent -> appendChild(this);
    } else {
        rootItemInit();
    }
}

ModelItem::ModelItem(QString file_path, ModelItem *parent, int init_state) {
    state = new ModelItemState(init_state);
    parentItem = parent;

    if (!state -> isUnprocessed()) {
        init(false);
        path = file_path.section('/', 0, -2);
        title = file_path.section('/', -1, -1);
        extension = title.section('.', -1, -1);
        if (extension != title)
            title = title.section('.', 0, -2);
        else extension = QString();

    } else {
        init(true);
        title = path = file_path;
        extension = QString();

        if (parent != 0)
            parent -> folders -> insert(title, this);
    }

    if (parent != 0) {
        if (!state -> isUnprocessed()) {
            parent -> insertChild(0, this);
        } else {
            parent -> appendChild(this);
        }
    }
}

ModelItem::~ModelItem() {
    qDeleteAll(childItems);

    delete state;

    delete folders;

    delete titlesCache;
}

/////////////////////////////////////////////////////////

ModelItem *ModelItem::parent() {
    return parentItem;
}

/////////////////////////////////////////////////////////

ModelItem *ModelItem::child(int row) {
    return childItems.value(row);
}

int ModelItem::childTreeCount() const {
    int ret = 0;
    foreach(ModelItem * childItem, childItems) {
        if (childItem -> folders == 0) // not is unprocessed
            ret += 1;
        else
            ret += childItem -> childTreeCount();
    }

    return ret;
}

int ModelItem::childCount() const {
    return childItems.count();
}

void ModelItem::insertChild(int pos, ModelItem *item) {
    childItems.insert(pos, item);
}

void ModelItem::appendChild(ModelItem *item) {
    childItems.append(item);
}

bool ModelItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

/////////////////////////////////////////////////////////

int ModelItem::column() const {
     return 0;
}

int ModelItem::columnCount() const {
     return 1;
}

/////////////////////////////////////////////////////////

int ModelItem::row() const {
     if (parentItem)
         return parentItem -> childItems.indexOf(const_cast<ModelItem*>(this));

     return 0;
}

/////////////////////////////////////////////////////////

QVariant ModelItem::data(int column) const {
    switch(column) {
        case TITLEID: return getTitle();
        case EXTENSIONID: return extension;
        case PATHID: return path;
        case FOLDERID: return state -> isUnprocessed(); // is folder
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

void ModelItem::dropExpandProceedFlags() {
    foreach(ModelItem * item, folders -> values()) {
        item -> getState() -> unsetProceed();
        item -> dropExpandProceedFlags();
    }
}

/////////////////////////////////////////////////////////

QString ModelItem::fullPath() const {
    ModelItem * curr = parentItem;
    QString path_buff = "";

    while(curr != 0) {
        path_buff = curr -> path + '/' + path_buff;
        curr = curr -> parentItem;
    }

    if (extension.isEmpty())
        return path_buff.mid(1) + title;
    else
        return path_buff.mid(1) + title + '.' + extension;
}

QString ModelItem::getTitle() const {
    return title;
}

void ModelItem::openLocation() {
    if (isFolder())
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath()));
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(parent() -> fullPath()));
}

bool ModelItem::isExist() const {
    return QFile::exists(fullPath());
}

bool ModelItem::isFolder() const {
    return false;
}

QHash<QString, ModelItem *> * ModelItem::foldersList() const {
    return folders;
}
int ModelItem::removeFolder(QString name) {
    return folders -> remove(name);
}

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
QList<QString> * ModelItem::getTitlesCache() const {
    return titlesCache;
}

//////////////////////////properties///////////////////////////////

ModelItemState * ModelItem::getState() const {
    return state;
}

void ModelItem::setState(int new_state, bool append_to_library) {
    if (state -> setBit(new_state) && append_to_library) {
        if (state -> isListened())
            Library::instance() -> addItem(this, STATE_LISTENED);
        else if (state -> isLiked())
            Library::instance() -> addItem(this, STATE_LIKED);
    }
}

///////////////////////////////////////////////////////

QJsonObject ModelItem::toJSON() {
    QJsonObject root = QJsonObject();

    if (childItems.length() > 0) {
        QJsonArray ar = QJsonArray();
        for(int i=0; i < childItems.length(); i++)
            ar.append(childItems.at(i)->toJSON());

        root["c"] = ar;
    }

    root["s"] = state -> getFuncValue();

    if (state -> isUnprocessed())
        root["p"] = path;
    else {
        root["n"] = title;
        root["e"] = extension;
    }

    return root;
}

//TODO: add list of extensions for extension serialization to extension index in list
