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
    inline void dropPackets() {
        mutex -> lock();
        while(packets.size() > 0)
            av_free_packet(packets.takeFirst());
        mutex -> unlock();
    }

    inline void pauseOnComplete() { finishAndPause = true; }
    inline void exitOnComplete() { finishAndExit = true; }

    bool seek(AVFormatContext * context, int64_t target, int flags);
    void resume();

    inline bool isValid() const { return state; }
    inline int index() const { return uindex; }

    virtual bool isBlocked() = 0;
    virtual void suspendOutput() = 0;
    virtual void resumeOutput() = 0;
protected:
    virtual void routine() {} // stub

    uint waitMillis;
    int packetsLimit;

    bool state, finishAndPause, finishAndExit;
    AVStream * stream;
    int uindex;
    AVCodecContext * codec_context;
    AVCodec * codec; // this var is possible to get through stream -> codec()
    AVFrame * frame;
    QList<AVPacket*> packets;
};


#endif // MEDIA_STREAM_H
