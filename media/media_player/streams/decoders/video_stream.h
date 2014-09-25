#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media/media_player/utils/video_frame.h"
#include "media/media_player/streams/base/media_stream.h"
#include "media/media_player/streams/output/video/base_output.h"
#include "media/media_player/resamplers/video_resampler.h"

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    bool isBlocked();
    void flushData();
    void suspendStream();
    void resumeStream();

    inline QWidget * getScreenWidget() const { return output -> widget(); }
protected:
    void routine();
    VideoFrame * calcPts(VideoFrame * videoFrame);
    double syncPts(AVFrame *src_frame);
    double calcAspectRatio();
private:   
    double aspect_ratio;

    BaseOutput * output;
    VideoResampler * resampler;
};

#endif // VIDEO_STREAM_H
