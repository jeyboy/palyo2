#ifndef AUDIO_FRAME_H
#define AUDIO_FRAME_H

#include <QByteArray>

struct AudioFrame {
    AudioFrame() {
        buffer = 0;
        bufferPTS = 0;
    }
    AudioFrame(QByteArray * data, double pts) {
        buffer = data;
        bufferPTS = pts;
    }

    ~AudioFrame() {
        delete buffer;
    }

    QByteArray * buffer;
    double bufferPTS;
};

#endif // AUDIO_FRAME_H
