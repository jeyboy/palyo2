#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

#define STATE_UNPROCESSED 128
#define STATE_DEFAULT 64
#define STATE_LISTENED 32
#define STATE_LIKED 16

#define STATE_PLAYED 8

#define STATE_NOT_EXIST 2
#define STATE_LIST_PROCEED 1

class ModelItemState {
public:
    ModelItemState();
    ModelItemState(int state);

    bool isNone();
    bool isListened();
    bool isLiked();

    bool isPlayed();
    bool isProceed();
    bool isNotExist();
    bool isUnprocessed();

    bool setNone();
    bool setListened();
    bool setLiked();

    bool setPlayed();
    bool unsetPlayed();

    bool setNotExist();
    bool unsetNotExist();

    bool setProceed();
    bool setUnprocessed();

    bool setBit(int val);

    int getValue() const;
    int getFuncValue();
    int currStateValue();

private:
    bool bitIsSet(int val, int pos);
    int setBit(int val, int pos);
    int unsetBit(int val, int pos);

    int item_state;
};


#endif // MODEL_ITEM_STATE_H
