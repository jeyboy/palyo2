#include "model.h"
#include <QDebug>

/////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * attrs, QObject *parent) : QAbstractItemModel(parent) {
    count = 0;
    if (attrs != 0) {
        rootItem = new ModelItem(attrs);
    } else
        rootItem = new ModelItem();
}

TreeModel::~TreeModel() {
//    delete rootItem;
}
/////////////////////////////////////////////////////////

QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    ModelItem * item;

    switch(role) {
        case Qt::DisplayRole: {
           item = getItem(index);

           if (!item -> getState() -> isProceed()) {
               Library::instance() ->initItem(item);
               qDebug() << item -> fullpath();
           }


           return item -> data(index.column());
        }
        case Qt::DecorationRole: {
           item = getItem(index);

           //QPixmap pixmap(26, 26);
           //pixmap.fill(color);
           //QIcon icon(pixmap);

           if (item -> getState() -> isUnprocessed())
               return IconProvider::fileIcon("", "");
           else
               return IconProvider::fileIcon(item -> fullpath(), (item -> data(EXTENSIONUID).toString()));
        }
        case Qt::ToolTipRole:
            item = getItem(index);
            return item -> data(EXTENSIONUID).toString();
        case Qt::SizeHintRole:
            return QSize(0, 18);
        case Qt::TextColorRole:
            return QColor(0, 0, 0);
        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
        case Qt::FontRole:
            return QFont("Times New Roman", 10, QFont::Bold);
        case Qt::UserRole:
            item = getItem(index);
//            qDebug() << item -> fullpath() << " | " << item -> getState() -> getValue() << " | " << item -> getState() -> getFuncValue();
            return item -> getState() -> currStateValue();

        default: return QVariant();
    }
}
bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole)
        return false;

    ModelItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

/////////////////////////////////////////////////////////

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEditable | Qt::ItemIsSelectable | QAbstractItemModel::flags(index);
}
QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    ModelItem *childItem = getItem(index);
    ModelItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

//    return createIndex(parentItem->row(), index.column(), parentItem);
    return createIndex(parentItem->row(), 0, parentItem);
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ModelItem *parentItem = getItem(parent);

    ModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::index(ModelItem * item) const {
   return createIndex(item -> row(), item -> column(), item);
}

/////////////////////////////////////////////////////////

int TreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<ModelItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

/////////////////////////////////////////////////////////

int TreeModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;

    ModelItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

void TreeModel::appendRow(QString filepath, ModelItem * parentItem) {
//    int position = parentItem -> childCount();
//    beginInsertRows(index(parentItem), position, position);
    new ModelItem(filepath, parentItem);
//    endInsertRows();

//    emit dataChanged(parent, parent);
}

bool TreeModel::removeRow(int row, const QModelIndex &parent) {
    if (parent.isValid()) {
        ModelItem * parentItem = getItem(parent);
        ModelItem * item = parentItem -> child(row);

        if (!item -> getState() -> isUnprocessed()) {
            beginRemoveRows(parent, row, row);
            if (parentItem -> removeChildren(row, 1)) {
                count--;
            }
            endRemoveRows();
            return true;
        }
    }

    return false;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
    ModelItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

/////////////////////////////////////////////////////////

void TreeModel::repaint() {
    beginResetModel();
    endResetModel();
}

void TreeModel::refreshItem(ModelItem * item) {
    QModelIndex ind = index(item);
    if (ind.isValid())
//        emit dataChanged(ind.parent(), ind);
        emit dataChanged(ind, ind);
}

/////////////////////////////////////////////////////////

ModelItem *TreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        ModelItem *item = static_cast<ModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

ModelItem * TreeModel::root() const {
    return rootItem;
}

/////////////////////////////////////////////////////////

//TODO: improve model insertion (add emit of rows insertion)
ModelItem * TreeModel::buildPath(QString path) {
    QStringList list = path.split('/', QString::SkipEmptyParts);
    ModelItem * curr = rootItem;

    foreach(QString piece, list) {
        if (curr -> folders -> contains(piece)) {
            curr = curr -> folders -> value(piece);
        } else {
            curr = new ModelItem(piece, curr, STATE_UNPROCESSED);
        }
    }

    return curr;
}

ModelItem * TreeModel::addFolder(QString folder_name, ModelItem * parent) {
    ModelItem * curr = parent;

    if (curr -> folders -> contains(folder_name)) {
        curr = curr -> folders -> value(folder_name);
    } else {
        curr = new ModelItem(folder_name, curr, STATE_UNPROCESSED);
    }

    return curr;
}

/////////////////////////////////////////////////////////

Qt::DropActions TreeModel::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const {
    QStringList types;
    types << "text/uri-list";
    return types;
}

QMimeData * TreeModel::mimeData(const QModelIndexList &indexes) const {
    QMimeData *mimeData = new QMimeData();
    QList<QUrl> list;
    ModelItem * temp;

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            temp = getItem(index);
            list.append(QUrl::fromLocalFile(temp -> fullpath()));
//            if (!temp -> getState() -> isUnprocessed())
//            tempData = temp -> fullpath().toUtf8();
        }
    }

//    mimeData -> setData(DnD::instance() -> listItems, encodedData);
//    qDebug() << encodedData;
//    qDebug() << mimeData -> data(DnD::instance() -> listItems);
    mimeData -> setUrls(list);
    return mimeData;
}
