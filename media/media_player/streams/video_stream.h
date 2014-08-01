#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media_stream.h"

#include <QImage>

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream() {}

    void suspendOutput();
    void resumeOutput();

    bool decodeFrame(unsigned char* bytes, int size);
protected:
    void routine();
private:
    AVFrame * RGBFrame;
    QList<QImage> videoBuffer;
};

#endif // VIDEO_STREAM_H
