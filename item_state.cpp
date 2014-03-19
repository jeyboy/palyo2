#include "item_state.h"
#include <QDebug>

ItemState::ItemState() {
    item_state = STATE_DEFAULT;
}
ItemState::ItemState(int state) {
    item_state = state;
}

bool ItemState::isNone() {
    return bitIsSet(item_state, STATE_DEFAULT);
}
bool ItemState::isListened() {
    return bitIsSet(item_state, STATE_LISTENED);
}
bool ItemState::isLiked() {
    return bitIsSet(item_state, STATE_LIKED);
}
bool ItemState::isPlayed() {
    return bitIsSet(item_state, STATE_PLAYED);
}
bool ItemState::isProceed() {
    return bitIsSet(item_state, STATE_LIST_PROCEED);
}
bool ItemState::isUnprocessed() {
    return bitIsSet(item_state, STATE_UNPROCESSED);
}

void ItemState::setNone() {
    item_state = STATE_DEFAULT;
}
void ItemState::setListened() {
    if (!isLiked() && !isUnprocessed())
//        item_state = setBit(item_state, STATE_LISTENED);
    //    item_state = unsetBit(item_state, STATE_DEFAULT);
        item_state = setBit(STATE_LISTENED, item_state & 0x7); // get three first bits
}
void ItemState::setLiked() {
    if (isUnprocessed()) return;
//    item_state = setBit(item_state, STATE_LIKED);
//    item_state = unsetBit(item_state, STATE_LISTENED);
//    item_state = unsetBit(item_state, STATE_DEFAULT);

    item_state = setBit(STATE_LIKED, item_state & 0x7); // get three first bits
}

void ItemState::setPlayed() {
    if (isUnprocessed()) return;
    item_state = setBit(item_state, STATE_PLAYED);
}
void ItemState::unsetPlayed() {
    item_state = unsetBit(item_state, STATE_PLAYED);
}

void ItemState::setProceed() {
    item_state = setBit(item_state, STATE_LIST_PROCEED);
}
void ItemState::setUnprocessed() {
    item_state = setBit(item_state, STATE_UNPROCESSED);
}

int ItemState::getValue() const {
    return item_state;
}

int ItemState::getFuncValue() {
    if (isPlayed())
        return STATE_PLAYED;
    else
        return item_state & ((0<<4)-1) << 4;
}

//////////// private /////////////////

bool ItemState::bitIsSet(int val, int pos) {
   return (val & pos) == pos;
}

int ItemState::setBit(int val, int pos) {
   return val | pos;
}
int ItemState::unsetBit(int val, int pos) {
   return val & (~(pos + 1));
}
