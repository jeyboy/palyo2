#include "model_item_state.h"
#include <QDebug>

ModelItemState::ModelItemState() {
    item_state = STATE_DEFAULT;
}
ModelItemState::ModelItemState(int state) {
    item_state = state;
}

bool ModelItemState::isNone() {
    return bitIsSet(item_state, STATE_DEFAULT);
}
bool ModelItemState::isListened() {
    return bitIsSet(item_state, STATE_LISTENED);
}
bool ModelItemState::isLiked() {
    return bitIsSet(item_state, STATE_LIKED);
}
bool ModelItemState::isPlayed() {
    return bitIsSet(item_state, STATE_PLAYED);
}
bool ModelItemState::isProceed() {
    return bitIsSet(item_state, STATE_LIST_PROCEED);
}
bool ModelItemState::isUnprocessed() {
    return bitIsSet(item_state, STATE_UNPROCESSED);
}

bool ModelItemState::setBit(int val) {
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

bool ModelItemState::setNone() {
    item_state = STATE_DEFAULT;
    return true;
}
bool ModelItemState::setListened() {
    if (!isLiked() && !isUnprocessed()) {
        item_state = setBit(STATE_LISTENED, item_state & 0x7); // get three first bits
        return true;
    }
    return false;
}
bool ModelItemState::setLiked() {
    if (isUnprocessed()) return false;

    item_state = setBit(STATE_LIKED, item_state & 0x7); // get three first bits
    return true;
}

bool ModelItemState::setPlayed() {
    if (isUnprocessed()) return false;
    item_state = setBit(item_state, STATE_PLAYED);
    return true;
}
bool ModelItemState::unsetPlayed() {
    item_state = unsetBit(item_state, STATE_PLAYED);
    return true;
}

bool ModelItemState::setProceed() {
    item_state = setBit(item_state, STATE_LIST_PROCEED);
    return true;
}
bool ModelItemState::setUnprocessed() {
    item_state = setBit(item_state, STATE_UNPROCESSED);
    return true;
}

int ModelItemState::getValue() const {
    return item_state;
}

int ModelItemState::getFuncValue() {
    return item_state & ((0<<4)-1) << 4;
}

int ModelItemState::currStateValue() {
    if (isPlayed())
        return STATE_PLAYED;
    else
        return item_state & ((0<<4)-1) << 4;
}

//////////// private /////////////////

bool ModelItemState::bitIsSet(int val, int pos) {
   return (val & pos) == pos;
}

int ModelItemState::setBit(int val, int pos) {
   return val | pos;
}
int ModelItemState::unsetBit(int val, int pos) {
   return val & (~(pos + 1));
}
