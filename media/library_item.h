#ifndef LIBRARY_PROC_ITEM_H
#define LIBRARY_PROC_ITEM_H

#include "model/model.h"
#include "model/model_item.h"

class Model;
class ModelItem;

class LibraryItem {
public:
    LibraryItem(const Model * item_model, ModelItem * proc_item);

    void refresh(int state);

    ModelItem * item() const;

private:

    Model * model;
    ModelItem * model_item;
};

#endif // LIBRARY_PROC_ITEM_H

