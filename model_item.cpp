#include "model_item.h"
#include <QDebug>

/////////////////////////////////////////////////////////

void ModelItem::init(bool isFolder) {
    names = 0;
    childItems = QList<ModelItem*>();

    if (isFolder) {
        state = new ModelItemState(STATE_UNPROCESSED);
        state -> setProceed();
        folders = new QHash<QString, ModelItem *>();
    } else {
        folders = 0;
    }
}

void ModelItem::rootItemInit() {
    path = QString();
    name = QString("--(O_o)--");
    extension = QString();
}

ModelItem::ModelItem() {
    init(true);
    rootItemInit();
    parentItem = 0;
}

ModelItem::ModelItem(QJsonObject * attrs, ModelItem *parent) {
    init(true);
    parentItem = parent;

    if (attrs -> contains("p")) {
        name = path = attrs -> value("p").toString();

        if (parent != 0)
            parent -> folders -> insert(name, this);
    } else {
        name = attrs -> value("n").toString();
        state = new ModelItemState(attrs -> value("s").toInt());
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
        name = file_path.section('/', -1, -1);
        extension = name.section('.', -1, -1);
        name = name.section('.', 0, -2);
    } else {
        init(true);
        name = path = file_path;
        extension = QString();

        if (parent != 0)
            parent -> folders -> insert(name, this);
    }

    if (parent != 0) { parent -> appendChild(this);}
}

ModelItem::~ModelItem() {
    qDeleteAll(childItems);

    delete state;

    delete folders;

    delete names;
}

/////////////////////////////////////////////////////////

ModelItem *ModelItem::parent() {
    return parentItem;
}

/////////////////////////////////////////////////////////

ModelItem *ModelItem::child(int row) {
    return childItems.value(row);
}

int ModelItem::childCount() const {
    return childItems.count();
}

void ModelItem::appendChild(ModelItem *item) {
    childItems.append(item);
}

bool ModelItem::insertChildren(int position, int count, int /*columns*/) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
//        QVector<QVariant> data(columns);
//        ModelItem *item = new ModelItem(data, this);
        ModelItem *item = new ModelItem(QString("BLA"), this);
        childItems.insert(position, item);
    }

    return true;
}

bool ModelItem::removeChildren(int position, int count)
{
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
//    if (parentItem == 0)
//        return 0;
//    else
//        return 1;
}

bool ModelItem::removeColumns(int /*position*/, int /*columns*/) {
//    if (position < 0 || position + columns > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.remove(position);

//    foreach (TreeItem *child, childItems)
//        child->removeColumns(position, columns);

    return true;
}

bool ModelItem::insertColumns(int /*position*/, int /*columns*/) {
//    if (position < 0 || position > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.insert(position, QVariant());

//    foreach (TreeItem *child, childItems)
//        child->insertColumns(position, columns);

    return true;
}

/////////////////////////////////////////////////////////

int ModelItem::row() const {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<ModelItem*>(this));

     return 0;
}

/////////////////////////////////////////////////////////

QVariant ModelItem::data(int column) const {
    switch(column) {
        case NAMEUID: return name;
        case EXTENSIONUID: return extension;
        case PATHUID: return path;
        default: return QVariant();
    }
}

bool ModelItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= 2)
//    if (column < 0 || column >= itemData.size())
        return false;

//    itemData[column] = value;

    switch(column) {
        case NAMEUID: name = value.toString();
        case EXTENSIONUID: extension = value.toString();
        case PATHUID: path = value.toString();
    }

//    itemData[column] = value;

    return true;
}

/////////////////////////////////////////////////////////

QString ModelItem::fullpath() const {
    ModelItem * curr = parentItem;
    QString path_buff = "";

    while(curr != 0) {
        path_buff = curr -> path + '/' + path_buff;
        curr = curr -> parentItem;
    }

    return path_buff.mid(1) + name + '.' + extension;
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
    // TODO: logic build needed
    QJsonObject root = QJsonObject();

    if (childItems.length() > 0) {
        QJsonArray ar = QJsonArray();
        for(int i=0; i < childItems.length(); i++)
            ar.append(childItems.at(i)->toJSON());

        root["c"] = ar;
    }

    if (state -> isUnprocessed())
        root["p"] = path;
    else {
        root["n"] = name;
        root["s"] = state -> getFuncValue();
        root["e"] = extension;
    }

    return root;
}

//TODO: add list of extensions for extension serialization to extension index in list
