#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "stream.h"

class MediaStream : public Stream {
    Q_OBJECT
public:
    MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority = InheritPriority);
    virtual ~MediaStream();

    inline bool isValid() const { return state; }
    inline uint index() const { return uindex; }

    virtual void suspendOutput() = 0;
    virtual void resumeOutput() = 0;
protected:
    virtual void routine() = 0;

    bool state;
    AVStream * stream;
    uint uindex;
    AVCodec * codec; // this var is possible to get through stream -> codec()
    AVFrame * frame;
};


#endif // MEDIA_STREAM_H
