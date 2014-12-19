#ifndef QUEUE_CELL_H
#define QUEUE_CELL_H

#include "media/media_player/utils/media_player_utils.h"

template <class T>
class QueueCell {
public:
    QueueCell(T value, QueueCell<T> * next_cell) : prev(0), next(next_cell), val(value) {
        if (next) {
            if (next -> prev) {
                next -> prev -> next = this;
                this -> prev = next -> prev;
            }

            next -> prev = this;
        }
    }

    QueueCell(QueueCell<T> * prev_cell, T value) : prev(prev_cell), next(0), val(value) {
        if (prev) {
            next = prev -> next;
            prev -> next = this;
        }

        if (next) {
            next -> prev = this;
        }
    }
    ~QueueCell() {

    }

    inline T value() { return val; }
    inline QueueCell<T> * getPrev() const { return prev; }
    inline QueueCell<T> * getNext() const { return next; }

    void dequeue() {
        if (next)
            next -> prev = prev;

        if (prev)
            prev -> next = next;
    }

private:
    QueueCell<T> * prev;
    QueueCell<T> * next;
    T val;
};

#endif // QUEUE_CELL_H
