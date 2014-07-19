#include "rand.h"
#include <QTime>

void Rand::shuffle(QList<ModelItem *> * list) {
    qsrand((uint)QTime::currentTime().msec());
    int n = list -> count() - 1;
    for (int i = 0; i < n; ++i) {
        int ro = randInt(i, n);
        list -> swap(i, ro);
    }
}

int Rand::randInt(int low, int high) {
    return qrand() % ((high + 1) - low) + low;
}
