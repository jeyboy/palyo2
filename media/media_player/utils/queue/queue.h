#ifndef QUEUE_H
#define QUEUE_H

#include "queue_cell.h"
#include <qmutex.h>

template <class T>
class Queue/* : public QObject*/ {
//    Q_OBJECT
public:
    Queue() : mutex(new QMutex()), root(0), tail(0), count(0) {

    }

    ~Queue() {
        clear();
        delete mutex;
    }

    void clear() {
        QueueCell<T> * iter;

        mutex -> lock();
        while(tail) {
            iter = tail -> getPrev();
            delete tail -> value();
            delete tail;
            tail = iter;
        }
        count = 0;
        mutex -> unlock();
    }

    void clear(void (*f)(T data)){
        QueueCell<T> * iter;

        mutex -> lock();
        while(tail) {
            iter = tail -> getPrev();
            f(tail -> value());
            delete tail;
            tail = iter;
        }
        count = 0;
        mutex -> unlock();
    }

    QueueCell<T> * append(T val) {
        mutex -> lock();
        tail = new QueueCell<T>(val, tail);
        if (!root) root = tail;
        count++;
        mutex -> unlock();
        return tail;
    }

    QueueCell<T> * prepend(T val) {
        mutex -> lock();
        root = new QueueCell<T>(val, 0);
        if (!root) root = tail;
        count++;
        mutex -> unlock();
        return tail;
    }

    T take(QueueCell<T> * cell) {
        mutex -> lock();
        T val = cell -> value();
        cell -> dequeue();

        if (cell == root) root = root -> getNext();
        else if (cell == tail) tail = tail -> getPrev();

        count--;
        mutex -> unlock();
        delete cell;
        return val;
    }

    T takeFirst() {
        return take(root);
    }

    inline int size() const {
        mutex -> lock();
        int c = count;
        mutex -> unlock();
        return c;
    }
    inline bool isEmpty() const {
        mutex -> lock();
        int c = count;
        mutex -> unlock();
        return c == 0;
    }

private:
    QMutex * mutex;
    QueueCell<T> * root, * tail;
    int count;
};

#endif // QUEUE_H
