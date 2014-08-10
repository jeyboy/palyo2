#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media/media_player/streams/media_stream.h"

#include <QImage>

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    void suspendOutput();
    void resumeOutput();
protected:
    void routine();
    double calcPts();
    double syncPts(AVFrame *src_frame, double pts);
private:
    AVFrame * RGBFrame;
    QList<QImage *> videoBuffer;

    unsigned char * RGBBuffer;
    SwsContext* resampleContext;
};

#endif // VIDEO_STREAM_H
