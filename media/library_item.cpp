#include "library_item.h"

LibraryItem::LibraryItem(const QModelIndex & ind, ModelItem * indItem) {
    index = ind;
    modelItem = indItem;
}

void LibraryItem::setState(int state) {
    modelItem -> setState(state, false);
}

ModelItem * LibraryItem::item() {
    return modelItem;
}

void LibraryItem::refresh(int state) {
    if (index.isValid()) {
        setState(state);
        //TODO: some troubles with repaint
//        emit const_cast<QAbstractItemModel *>(index.model()) -> layoutChanged();
        emit const_cast<QAbstractItemModel *>(index.model()) -> dataChanged(index.parent(), index);
//        emit const_cast<QAbstractItemModel *>(index.model()) -> dataChanged(index, index);
//        emit const_cast<QAbstractItemModel *>(index.model()) -> dataChanged(index, index);
    } else {
        qDebug() << "Index is not valid";
    }
}
