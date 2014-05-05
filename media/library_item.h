#ifndef LIBRARY_ITEM_H
#define LIBRARY_ITEM_H

#include <QModelIndex>
#include "model/model_item.h"

class ModelItem;

class LibraryItem {
public:
    LibraryItem(const QModelIndex & ind, ModelItem * item);

    void setState(int state);

    void refresh(int state);

    ModelItem * item();

private:
    QModelIndex index;
    ModelItem * modelItem;
};

#endif // LIBRARY_ITEM_H

