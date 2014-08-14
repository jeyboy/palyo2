#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QImage>

struct VideoFrame {
    VideoFrame() {
        image = new QImage(":play");
        interval = 40; //millis
    }

    VideoFrame(QImage * img, uint frameInterval) {
        image = img;
        interval = frameInterval;
    }

    ~VideoFrame() {
        delete image;
    }

    QImage * image;
    uint interval;
};

#endif // VIDEO_FRAME_H
