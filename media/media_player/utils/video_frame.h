#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QTransform>
#include "media/media_player/utils/master_clock.h"
#include "media/media_player/utils/video_buffer.h"

struct VideoFrame {
    VideoFrame() {
        buffer = 0;
        pts = -1;
    }

    VideoFrame(VideoBuffer * buff, double framePts, double nextPts, double aspectRatio) {
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

    uint calcDelay() {
        uint res = 40;

        if (pts > -1) {
            res = MasterClock::instance() -> computeVideoDelay(
                pts == 0 ? MasterClock::instance() -> video() : pts,
                MasterClock::instance() -> video() + next_pts
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

    VideoBuffer * buffer;
    double pts, next_pts, aspect_ratio;
};

#endif // VIDEO_FRAME_H
