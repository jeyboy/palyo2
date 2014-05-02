#include "library_item.h"

LibraryItem::LibraryItem(const Model * item_model, ModelItem * proc_item) {
    model = const_cast<Model *>(item_model);
    model_item = proc_item;
}

void LibraryItem::refresh(int state) {
    model_item -> setState(state, false);
    model -> refreshItem(model_item);
}

ModelItem * LibraryItem::item() const {
    return model_item;
}
