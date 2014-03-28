#ifndef ITEM_STATE_H
#define ITEM_STATE_H

#define STATE_UNPROCESSED 128
#define STATE_DEFAULT 64
#define STATE_LISTENED 32
#define STATE_LIKED 16

#define STATE_PLAYED 8

#define STATE_LIST_PROCEED 1

#include "model_item.h"

class ModelItem;

class ItemState {
public:
    ItemState();
    ItemState(int state);

    bool isNone();
    bool isListened();
    bool isLiked();

    bool isPlayed();
    bool isProceed();
    bool isUnprocessed();

    void setNone();
    void setListened(ModelItem * item = 0);
    void setLiked(ModelItem * item = 0);

    void setPlayed();
    void unsetPlayed();
    void setProceed();
    void setUnprocessed();

    int getValue() const;
    int getFuncValue();
    int currStateValue();

private:
    bool bitIsSet(int val, int pos);
    int setBit(int val, int pos);
    int unsetBit(int val, int pos);

    int item_state;
};


#endif // ITEM_STATE_H
