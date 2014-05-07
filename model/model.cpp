#include "model.h"
#include <QDebug>


Model::Model(QJsonObject *hash, QObject *parent) : QAbstractItemModel(parent) {
    if (hash != 0) {
        rootItem = new FolderItem(hash);
        count = hash -> value("l").toInt();
    } else {
        rootItem = new FolderItem();
        count = 0;
    }
}

Model::~Model() {
//    delete rootItem;
}

QVariant Model::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    ModelItem * item;

    switch(role) {
        case Qt::DisplayRole: {
           item = getItem(index);

           qDebug() << item -> fullPath();

           if (!item -> getState() -> isProceed()) {
               item -> proceedByLibrary(index);

               if (item -> getState() -> isExpanded()) {
                   emit expandNeeded(index);
               }
           }

           return item -> data(index.column());
        }
        case Qt::DecorationRole: {
           item = getItem(index);

           //QPixmap pixmap(26, 26);
           //pixmap.fill(color);
           //QIcon icon(pixmap);

           if (item -> getState() -> isNotExist())
               return IconProvider::missedIcon();
           else if (item -> isFolder())
               return QVariant();
//               return IconProvider::fileIcon("", "");
           else
               return IconProvider::fileIcon(item -> fullPath(), (item -> data(EXTENSIONID).toString()));
        }
        case Qt::ToolTipRole:
            item = getItem(index);
            return item -> data(EXTENSIONID).toString();
        case Qt::SizeHintRole:
            return QSize(0, 18);
        case Qt::TextColorRole:
            return QColor(0, 0, 0);
        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
        case Qt::FontRole:
            return QFont("Arial", 9, QFont::Normal);
        case Qt::UserRole:
            item = getItem(index);
            return item -> getState() -> currStateValue();

        default: return QVariant();
    }
}
bool Model::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole)
        return false;

    ModelItem *item = getItem(index);
    bool result = item -> setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}
bool Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEditable | Qt::ItemIsSelectable | QAbstractItemModel::flags(index);
}
QModelIndex Model::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    ModelItem *childItem = getItem(index);
    ModelItem *parentItem = childItem -> parent();

    if (parentItem == rootItem)
        return QModelIndex();

//    return createIndex(parentItem->row(), index.column(), parentItem);
    return createIndex(parentItem -> row(), 0, parentItem);
}
QModelIndex Model::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModelItem *parentItem = getItem(parent);

    ModelItem *childItem = parentItem -> child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex Model::index(ModelItem * item) const {
   return createIndex(item -> row(), item -> column(), item);
}


int Model::itemsCount() const {
    return count;
}


int Model::columnCount(const QModelIndex &parent) const {
    return getItem(parent) -> columnCount();
//    if (parent.isValid())
//        return static_cast<ModelItem *>(parent.internalPointer()) -> columnCount();
//    else
//        return rootItem -> columnCount();
}

int Model::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;

    ModelItem *parentItem = getItem(parent);
    return parentItem -> childCount();
}

void Model::appendRow(ModelItem * item) {
//    int position = parentItem -> childCount();
//    beginInsertRows(index(parentItem), position, position);
    if (!item -> isFolder())
        emit itemsCountChanged(++count);
//    endInsertRows();

//    emit dataChanged(parent, parent);
}

bool Model::removeRow(int row, const QModelIndex &parentIndex) {
    int removeCount = 1;
    ModelItem * parentItem = getItem(parentIndex);
    ModelItem * item = parentItem -> child(row);
    QString folderName;
    bool isUnprocessed = item -> isFolder();
    bool res;

    if (isUnprocessed) {      
        folderName = item -> data(TITLEID).toString();

        removeCount = item -> childTreeCount();
    }

    beginRemoveRows(parentIndex, row, row);
    res = parentItem -> removeChildren(row, 1);
    endRemoveRows();

    if (res) {
        if (isUnprocessed) {
            parentItem -> removeFolder(folderName);
        }

        if (removeCount > 0)
            emit itemsCountChanged(count -= removeCount);
    }

    return res;
}

bool Model::removeRows(int position, int rows, const QModelIndex &parent) {
    ModelItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem -> removeChildren(position, rows);
    endRemoveRows();

    if (success)
        emit itemsCountChanged((count -= rows));

    return success;
}


void Model::refresh() {
    beginResetModel();
    endResetModel();
}

void Model::refreshItem(ModelItem * item) {
    QModelIndex ind = index(item);
    if (ind.isValid()) {
//        emit dataChanged(ind.parent(), ind);
        emit dataChanged(ind, ind);
    }
}

/////////////////////////////////////////////////////////

ModelItem * Model::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        return static_cast<ModelItem *>(index.internalPointer());
    }
    return rootItem;
}

//template<class T> T * Model::getItem(const QModelIndex &index) const {
//    if (index.isValid()) {
//        return dynamic_cast<T *>(index.internalPointer());
//    }
//    return dynamic_cast<T *>(rootItem);
//}

ModelItem * Model::root() const {
    return rootItem;
}

/////////////////////////////////////////////////////////

ModelItem * Model::buildPath(QString path) {
    QStringList list = path.split('/', QString::SkipEmptyParts);
    ModelItem * curr = rootItem;

    foreach(QString piece, list) {
        curr = addFolder(piece, curr);
    }

    return curr;
}

//TODO: improve model insertion (add emit of rows insertion)


ModelItem * Model::addFolder(QString folderPath, QString folderName, ModelItem * parent) {
    ModelItem * curr = parent;

    if (curr -> foldersList() -> contains(folderName)) {
        curr = curr -> foldersList() -> value(folderName);
    } else {
        curr = new FolderItem(folderPath, folderName, curr);
    }

    return curr;
}

ModelItem * Model::addFolder(QString folderPath, ModelItem * parent) {
    return addFolder(folderPath, folderPath, parent);
}

//////////////////////// slots //////////////////////////

void Model::expanded(const QModelIndex &index) {
    ModelItem * item = getItem(index);
    item -> getState() -> setExpanded();
}
void Model::collapsed(const QModelIndex &index) {
    ModelItem * item = getItem(index);
    item -> getState() -> unsetExpanded();
}

/////////////////////////////////////////////////////////

Qt::DropActions Model::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList Model::mimeTypes() const {
    QStringList types;
    types << "text/uri-list";
    return types;
}

QMimeData * Model::mimeData(const QModelIndexList &indexes) const {
    QMimeData *mimeData = new QMimeData();
    QList<QUrl> list;
    ModelItem * temp;

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            temp = getItem(index);
            list.append(temp -> toUrl());
        }
    }

    mimeData -> setUrls(list);
    return mimeData;
}
