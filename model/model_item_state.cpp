#include "model_item_state.h"
#include <QDebug>

ModelItemState::ModelItemState() {
    item_state = STATE_DEFAULT | STATE_CHECKED;
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
bool ModelItemState::isNotExist() {
    return bitIsSet(item_state, STATE_NOT_EXIST);
}
bool ModelItemState::isUnprocessed() {
    return bitIsSet(item_state, STATE_UNPROCESSED);
}
bool ModelItemState::isExpanded() {
    return bitIsSet(item_state, STATE_EXPANDED);
}

bool ModelItemState::isChecked() {
    return bitIsSet(item_state, STATE_CHECKED);
}

bool ModelItemState::setBit(int val) {
    bool result_state = true;

    if (val < 0) {
        if (bitIsSet(val, STATE_PLAYED)) {
            result_state &= unsetPlayed();
        }

        if (bitIsSet(val, STATE_NOT_EXIST)) {
           result_state &= unsetNotExist();
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

        if (bitIsSet(val, STATE_NOT_EXIST)) {
           result_state &= setNotExist();
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
        item_state = setBit(STATE_LISTENED, item_state & 31); // get five first bits
        return true;
    }
    return false;
}
bool ModelItemState::setLiked() {
    if (isUnprocessed()) return false;

    item_state = setBit(STATE_LIKED, item_state & 31); // get five first bits
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

bool ModelItemState::setExpanded() {
    if (!isUnprocessed()) return false;
    item_state = setBit(item_state, STATE_EXPANDED);
    return true;
}
bool ModelItemState::unsetExpanded() {
    item_state = unsetBit(item_state, STATE_EXPANDED);
    return true;
}


bool ModelItemState::setNotExist() {
    if (isUnprocessed()) return false;
    item_state = setBit(item_state, STATE_NOT_EXIST);
    return true;
}
bool ModelItemState::unsetNotExist() {
    item_state = unsetBit(item_state, STATE_NOT_EXIST);
    return true;
}

bool ModelItemState::setProceed() {
    item_state = setBit(item_state, STATE_LIST_PROCEED);
    return true;
}

bool ModelItemState::unsetProceed() {
    item_state = unsetBit(item_state, STATE_LIST_PROCEED);
    return true;
}


bool ModelItemState::setUnprocessed() {
    item_state = setBit(item_state, STATE_UNPROCESSED);
    return true;
}

bool ModelItemState::setChecked() {
    item_state = setBit(item_state, STATE_CHECKED);
    return true;
}
bool ModelItemState::unsetChecked() {
    item_state = unsetBit(item_state, STATE_CHECKED);
    return true;
}

int ModelItemState::getValue() const {
    return item_state;
}

int ModelItemState::getFuncValue() {
    return item_state & ((0 << 5) - 1) << 3;
}

int ModelItemState::currStateValue() {
    if (isPlayed())
        return STATE_PLAYED;
    else
        return item_state & ((0 << 3) - 1) << 5;
}

//////////// private /////////////////

bool ModelItemState::bitIsSet(int val, int pos) {
   return (val & pos) == pos;
}

int ModelItemState::setBit(int val, int pos) {
   return val | pos;
}
int ModelItemState::unsetBit(int val, int pos) {
    return val & (~(pos));
}
