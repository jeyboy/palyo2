#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "stream.h"
#include "i_media_stream.h"

class MediaStream : public Stream, public IMediaStream {
    Q_OBJECT
public:
    MediaStream(AVFormatContext * context, MasterClock * clock, int streamIndex, QObject * parent, Priority priority = InheritPriority);
    virtual ~MediaStream();

    inline bool requirePreload() { return !valid || (valid && packets.isEmpty()); }
    inline bool dataLoadingFinished() { return isSuspended() && eof;}

    void suspend();
signals:
    void suspended();

public slots:
    virtual void nextFrame(void *& ret) { ret = 0; } // stub
    void rejectEof();
    void eofDetected();
    virtual void suspendStream();
    virtual void resumeStream();
    virtual void flushData() {}

protected:
    virtual void routine() {} // stub
    MasterClock * clock;

    bool eof;
};


#endif // MEDIA_STREAM_H
