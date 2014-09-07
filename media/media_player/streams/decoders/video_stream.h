#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media/media_player/utils/video_frame.h"
#include "media/media_player/streams/base/media_stream.h"
#include "media/media_player/streams/output/video/video_output.h"
#include "media/media_player/resamplers/video_resampler.h"

class VideoStream : public MediaStream {
    Q_OBJECT
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    bool isBlocked();
    void flushData();
    void resumeStream();
public slots:
    void nextPict();
protected:
    void routine();
    VideoFrame * calcPts(VideoFrame * videoFrame);
    double syncPts(AVFrame *src_frame);
    double calcAspectRatio();
private:   
    VideoOutput * output;
    VideoResampler * resampler;
    QList<VideoFrame *> frames;
};

#endif // VIDEO_STREAM_H
