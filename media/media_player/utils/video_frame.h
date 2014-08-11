#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QImage>

struct VideoFrame {
    VideoFrame() {
        image = new QImage(":play");
        interval = 40;
    }

    VideoFrame(QImage * img, double frameInterval) {
        image = img;
        interval = frameInterval;
    }

    ~VideoFrame() {
        delete image;
    }

    QImage * image;
    double interval;
};

#endif // VIDEO_FRAME_H
