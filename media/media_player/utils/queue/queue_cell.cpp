#include "queue_cell.h"

template <class T> QueueCell<T>::QueueCell(T * value, QueueCell<T> * prev_cell) : prev(0), next(0), val(value) {
    if (prev = prev_cell) {
        next = prev -> next;
        prev -> next = this;
    }

    if (next) {
        next -> prev = this;
    }
}

template <class T> QueueCell<T>::~QueueCell() {
//    dequeue();
}

template <class T> void QueueCell<T>::dequeue() {
    if (next)
        next -> prev = prev;

    if (prev)
        prev -> next = next;
}
