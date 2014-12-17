#include "queue.h"

template <class T> Queue<T>::Queue() : tail(0), count(0) {

}

template <class T> Queue<T>::~Queue() {
    QueueCell<T> * iter;
    while(tail) {
        iter = tail -> prev;
    }
}

template <class T> QueueCell<T> * Queue<T>::add(T * val) {
    tail = new QueueCell<T>(val, tail);
    count++;
    return tail;
}

template <class T> T * Queue<T>::take(QueueCell<T> * cell) {
    T * val = cell -> value();
    cell -> dequeue();
    delete cell;
    count--;
    return val;
}
