#ifndef API_FUNC_CONTAINER_H
#define API_FUNC_CONTAINER_H

#include "misc/func_container.h"

struct ApiFuncContainer {
    ApiFuncContainer() { }
    ApiFuncContainer(const FuncContainer & container, const QString & uid) {
        func = container;
        this -> uid = uid;
    }
    ~ApiFuncContainer() {}

    const QString uid;
    const FuncContainer func;
};

#endif // API_FUNC_CONTAINER_H
