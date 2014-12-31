#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "../frames/video_frame.h"
#include "media/media_player/streams/base/media_stream.h"
#include "media/media_player/resamplers/video_resampler.h"
#include "media/media_player/output/video/video_output.h"

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    int calcDelay();

    bool isBlocked();
    void flushData();
    void suspendStream();
    void resumeStream();

    inline QWidget * getScreenWidget() const { return output; }
    void changeRenderType(RenderType type);

    void nextFrame(void *&);

protected:
    void prepareRenderType(RenderType newType);
    void routine();
    VideoFrame * calcPts(VideoFrame * videoFrame);
    double syncPts(AVFrame *src_frame);
    void calcAspectRatio();
private:   
    double aspect_ratio;

    VideoOutput * output;
    QList<VideoFrame *> frames;
    VideoResampler * resampler;
    RenderType type;
};

#endif // VIDEO_STREAM_H
