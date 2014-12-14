#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QTransform>
#include "media/media_player/utils/master_clock.h"
#include "./video/video_buffer.h"

struct VideoFrame {
    VideoFrame(MasterClock * clock) : clock(clock) {
        buffer = 0;
        pts = -1;
    }

    VideoFrame(MasterClock * clock, VideoBuffer * buff, double framePts, double nextPts, double aspectRatio) : clock(clock) {
        aspect_ratio = aspectRatio;
        buffer = buff;
        pts = framePts;
        next_pts = nextPts;
    }

    ~VideoFrame() {
        delete buffer;
    }

    QImage * asImage() { return buffer -> asQImage(); }
    AVPicture * asPicture() { return buffer -> asAVPicture(); }

    bool skip() { return buffer == 0;}

    uint calcDelay() {
        uint res = 40;

        if (pts > -1) {
            res = clock -> computeVideoDelay(
                pts == 0 ? clock -> video() : pts,
                clock -> video() + next_pts
            );
        }

        if (pts < -1)
            res = -pts;

        return res;
    }

    QRect calcSize(QRect defSize) {
        int h = defSize.height();
        int w = ((int)rint(h * aspect_ratio)) & -3;
        if (w > defSize.width()) {
            w = defSize.width();
            h = ((int)rint(w / aspect_ratio)) & -3;
        }

        QRect rect(defSize.left() + (defSize.width() - w) / 2, defSize.top() + (defSize.height() - h) / 2, w, h);
        return rect;
    }

    MasterClock * clock;
    VideoBuffer * buffer;
    double pts, next_pts, aspect_ratio;
};

#endif // VIDEO_FRAME_H
