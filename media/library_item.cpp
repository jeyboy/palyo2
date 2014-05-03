#include "library_item.h"

LibraryItem::LibraryItem(const QModelIndex & ind) {
    model = dynamic_cast<Model *>(const_cast<QAbstractItemModel *>(ind.model()));
    model_item = model -> getItem(ind);
}

void LibraryItem::refresh(int state) {
    model_item -> setState(state, false);
    model -> refreshItem(model_item);
}

ModelItem * LibraryItem::item() const {
    return model_item;
}
