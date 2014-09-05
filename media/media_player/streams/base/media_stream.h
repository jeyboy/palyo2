#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "stream.h"
#include "i_media_stream.h"

class MediaStream : public Stream, public IMediaStream {
public:
    MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority = InheritPriority);
    virtual ~MediaStream();

    inline bool requirePreload() { return !valid || (valid && packets.isEmpty()); }
protected:
    virtual void routine() {} // stub

    uint waitMillis;
};


#endif // MEDIA_STREAM_H
