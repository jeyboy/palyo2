#ifndef QUEUE_CELL_H
#define QUEUE_CELL_H

#include "media/media_player/utils/media_player_utils.h"

template <class T>
class QueueCell {
public:
    QueueCell(T * value, QueueCell<T> * prev_cell);
    ~QueueCell();

    inline T * value() { return val; }

    void dequeue();

private:
    QueueCell<T> * prev;
    QueueCell<T> * next;
    T * val;
};

#endif // QUEUE_CELL_H
