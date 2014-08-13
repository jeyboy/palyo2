#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media/media_player/utils/video_frame.h"
#include "media/media_player/streams/media_stream.h"
#include "media/media_player/streams/output/video/gl_output.h"
#include "media/media_player/resamplers/video_resampler.h"

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    inline bool isBlocked() { return isValid() && output -> millisPreloaded() > 1; }
    void suspendOutput();
    void resumeOutput();
protected:
    void routine();
    double calcPts();
    double syncPts(AVFrame *src_frame, double pts);
private:
    GLOutput * output;
    VideoResampler * resampler;
};

#endif // VIDEO_STREAM_H
