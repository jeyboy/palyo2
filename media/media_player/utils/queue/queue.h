#ifndef QUEUE_H
#define QUEUE_H

#include "queue_cell.h"
#include <qmutex.h>

template <class T>
class Queue/* : public QObject*/ {
//    Q_OBJECT
public:
    Queue();

    ~Queue();

    QueueCell<T> * add(T * val);
    T * take(QueueCell<T> *);
    inline int size() const { return count; }

private:
    QMutex mutex;
    QueueCell<T> * tail;
    int count;
};

#endif // QUEUE_H
