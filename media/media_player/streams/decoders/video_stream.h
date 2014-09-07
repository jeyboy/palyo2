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
private:   
    VideoOutput * output;
    VideoResampler * resampler;
    QList<VideoFrame *> frames;
};

#endif // VIDEO_STREAM_H


//if(vp->bmp) {
//  if(is->video_st->codec->sample_aspect_ratio.num == 0) {
//    aspect_ratio = 0;
//  } else {
//    aspect_ratio = av_q2d(is->video_st->codec->sample_aspect_ratio) *
//  is->video_st->codec->width / is->video_st->codec->height;
//  }
//  if(aspect_ratio <= 0.0) {
//    aspect_ratio = (float)is->video_st->codec->width /
//  (float)is->video_st->codec->height;
//  }
//  h = screen->h;
//  w = ((int)rint(h * aspect_ratio)) & -3;
//  if(w > screen->w) {
//    w = screen->w;
//    h = ((int)rint(w / aspect_ratio)) & -3;
//  }
