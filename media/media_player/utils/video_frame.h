#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QImage>
#include "media/media_player/utils/master_clock.h"

struct VideoFrame {
    VideoFrame() {
        image = new QImage(":play");
        pts = -1;
    }

    VideoFrame(QImage * img, double framePts, double nextPts) {
        image = img;
        pts = framePts;
        next_pts = nextPts;
    }

    ~VideoFrame() {
        delete image;
    }

    uint calcDelay() {
        if (pts == -1)
            return 40;
        else
            MasterClock::instance() -> computeVideoDelay(
                pts == 0 ? MasterClock::instance() -> video() : pts,
                MasterClock::instance() -> video() + next_pts
            );
    }

    QImage * image;
    double pts, next_pts;
};

#endif // VIDEO_FRAME_H
