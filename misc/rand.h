#ifndef RAND_H
#define RAND_H

#include <QList>
#include "model/model_item.h"

class Rand {
public:
    static void shuffle(QList<ModelItem *> * list);
protected:
    static inline int randInt(int low, int high);
};

#endif // RAND_H
