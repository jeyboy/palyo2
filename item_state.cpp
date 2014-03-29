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

bool ItemState::setBit(int val) {
    bool result_state = true;

    if (val < 0) {
        if (bitIsSet(val, STATE_PLAYED)) {
           result_state &= unsetPlayed();
        }
    } else {
        if (bitIsSet(val, STATE_LISTENED)) {
            result_state &= setListened();
        }

        if (bitIsSet(val, STATE_LIKED)) {
            result_state &= setLiked();
        }

        if (bitIsSet(val, STATE_PLAYED)) {
            result_state &= setPlayed();
        }

        if (bitIsSet(val, STATE_LIST_PROCEED)) {
            result_state &= setProceed();
        }
    }

    return result_state;
}

bool ItemState::setNone() {
    item_state = STATE_DEFAULT;
    return true;
}
bool ItemState::setListened() {
    if (!isLiked() && !isUnprocessed()) {
        item_state = setBit(STATE_LISTENED, item_state & 0x7); // get three first bits
        return true;
    }
    return false;
}
bool ItemState::setLiked() {
    if (isUnprocessed()) return false;

    item_state = setBit(STATE_LIKED, item_state & 0x7); // get three first bits
    return true;
}

bool ItemState::setPlayed() {
    if (isUnprocessed()) return false;
    item_state = setBit(item_state, STATE_PLAYED);
    return true;
}
bool ItemState::unsetPlayed() {
    item_state = unsetBit(item_state, STATE_PLAYED);
    return true;
}

bool ItemState::setProceed() {
    item_state = setBit(item_state, STATE_LIST_PROCEED);
    return true;
}
bool ItemState::setUnprocessed() {
    item_state = setBit(item_state, STATE_UNPROCESSED);
    return true;
}

int ItemState::getValue() const {
    return item_state;
}

int ItemState::getFuncValue() {
    return item_state & ((0<<4)-1) << 4;
}

int ItemState::currStateValue() {
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
