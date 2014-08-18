#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "stream.h"
#include "i_media_stream.h"

class MediaStream : public Stream, public IMediaStream {
public:
    MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority = InheritPriority);
    virtual ~MediaStream();

    inline bool requirePreload() { return !valid || (valid && packets.isEmpty()); }
    inline void pauseOnComplete() { finishAndPause = true; }
    inline void exitOnComplete() { finishAndExit = true; }

    void resume();

protected:
    virtual void routine() {} // stub

    uint waitMillis;

    bool finishAndPause, finishAndExit;
};


#endif // MEDIA_STREAM_H
