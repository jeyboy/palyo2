#ifndef QUEUE_H
#define QUEUE_H

#include "queue_cell.h"
#include <qmutex.h>
#include <qdebug.h>

template <class T>
class Queue/* : public QObject*/ {
//    Q_OBJECT
public:
    Queue() : mutex(new QMutex()), root(new QueueCell<T>((QueueCell<T> *)0, 0)), tail(new QueueCell<T>(root, 0)), count(0) {
    }

    ~Queue() {
        clear();
        delete mutex;
        delete root;
        delete tail;
    }

    void clear() {
        QueueCell<T> * iter = tail -> getPrev(), * temp;

        mutex -> lock();
        while(iter != root) {
            temp = iter -> getPrev();
            delete iter -> value();
            delete iter;
            iter = temp;
        }
        count = 0;
        mutex -> unlock();
    }

    void clear(void (*f)(T data)){
        QueueCell<T> * iter = tail -> getPrev(), * temp;

        mutex -> lock();
        while(iter != root) {
            temp = iter -> getPrev();
            f(tail -> value());
            delete iter;
            iter = temp;
        }
        count = 0;
        mutex -> unlock();
    }

    QueueCell<T> * append(T val) {
        mutex -> lock();
        new QueueCell<T>(val, tail);
        count++;
        mutex -> unlock();
        return tail;
    }

    QueueCell<T> * prepend(T val) {
        mutex -> lock();
        new QueueCell<T>(root, val);
        count++;
        mutex -> unlock();
        return root -> getNext();
    }

    T take(QueueCell<T> * cell) {
        if (cell == root || cell == tail) return 0;

        mutex -> lock();
        T val = cell -> value();
        cell -> dequeue();
        count--;
        mutex -> unlock();
        delete cell;
        return val;
    }

    T takeFirst() {
        return take(root -> getNext());
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
