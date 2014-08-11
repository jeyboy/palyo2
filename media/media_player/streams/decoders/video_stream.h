#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media/media_player/utils/video_frame.h"
#include "media/media_player/streams/media_stream.h"
#include "media/media_player/streams/output/video/gl_output.h"

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    void suspendOutput();
    void resumeOutput();

    double millisPreloaded();
protected:
    void routine();
    double calcPts();
    double syncPts(AVFrame *src_frame, double pts);
private:
    AVFrame * RGBFrame;

    unsigned char * RGBBuffer;
    SwsContext* resampleContext;

    GLOutput * output;
};

#endif // VIDEO_STREAM_H
