#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QImage>
#include <QPixmap>
#include <QTransform>
#include "media/media_player/utils/master_clock.h"

struct VideoFrame {
    VideoFrame() {
        image = 0;
        pts = -1;
    }

    VideoFrame(void * img, double framePts, double nextPts, double aspectRatio) {
        aspect_ratio = aspectRatio;
        image = img;
        pts = framePts;
        next_pts = nextPts;
    }

    ~VideoFrame() {
        QImage * img = asImage();
        if (img != 0)
            delete asImage();
        else
            delete asFrame();
    }

    QImage * asImage() { return (QImage *)image; }
    AVFrame * asFrame() { return (AVFrame *)image; }

    uint calcDelay() {
        uint res = 40;

        if (pts > -1)
            res = MasterClock::instance() -> computeVideoDelay(
                pts == 0 ? MasterClock::instance() -> video() : pts,
                MasterClock::instance() -> video() + next_pts
            );

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

    void * image;
    double pts, next_pts, aspect_ratio;
};

#endif // VIDEO_FRAME_H
