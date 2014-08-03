#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "stream.h"

class MediaStream : public Stream {
    Q_OBJECT
public:
    MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority = InheritPriority);
    virtual ~MediaStream();

    void decode(AVPacket * newPacket);
//    void decode(unsigned char* bytes, int size);

    void pauseOnComplete();

    void resume();

    inline bool isValid() const { return state; }
    inline int index() const { return uindex; }

    inline double getClock() { return clock; }
    inline void setClock(double newClock) { clock = newClock; }
    inline void iterateClock(double offset) { clock += offset; }

    virtual void suspendOutput() = 0;
    virtual void resumeOutput() = 0;
protected:
    virtual void routine() {} // stub

    double clock;

    bool state, finishAndPause;
    AVStream * stream;
    int uindex;
    int bufferLimit;
    AVCodecContext * codec_context;
    AVCodec * codec; // this var is possible to get through stream -> codec()
    AVFrame * frame;
    QList<AVPacket*> packets;
};


#endif // MEDIA_STREAM_H
