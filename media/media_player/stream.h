#ifndef STREAM_H
#define STREAM_H

#include "media_player_utils.h"

class Stream {
public:
    Stream(AVFormatContext * context, uint streamIndex);
    ~Stream();

    inline bool isValid() const { return state; }
    inline uint index() const { return uindex; }
private:
    bool state;
    AVStream * stream;
    uint uindex;
    AVCodec * codec; // this var is possible to get through stream -> codec()
};

#endif // STREAM_H
