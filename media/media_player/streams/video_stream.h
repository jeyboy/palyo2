#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media_stream.h"

#include <QImage>

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    void suspendOutput();
    void resumeOutput();
protected:
    void routine();
private:
    AVFrame * RGBFrame;
    QList<QImage *> videoBuffer;

    unsigned char * RGBBuffer;
    SwsContext* resampleContext;
};

#endif // VIDEO_STREAM_H
