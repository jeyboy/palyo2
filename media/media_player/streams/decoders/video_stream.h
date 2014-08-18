#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "media/media_player/utils/video_frame.h"
#include "media/media_player/streams/base/media_stream.h"
#include "media/media_player/streams/output/video/video_output.h"
#include "media/media_player/resamplers/video_resampler.h"

class VideoStream : public MediaStream {
public:
    VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~VideoStream();

    inline bool isBlocked() { return isValid() && packets.size() > packetsLimit; }
    void suspendOutput();
    void resumeOutput();

    void stop();
protected:
    void routine();
    double calcPts();
    double syncPts(AVFrame *src_frame, double pts);
private:
    VideoOutput * output;
    VideoResampler * resampler;
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
