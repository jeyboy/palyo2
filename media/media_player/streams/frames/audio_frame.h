#ifndef AUDIO_FRAME_H
#define AUDIO_FRAME_H

#include <QByteArray>
#include <QAudioFormat>

struct AudioFrame {
    AudioFrame() {
        buffer = new QByteArray(4096, 0);
        bufferPTS = 0;
        time_length = 0;
    }
    AudioFrame(QByteArray * data, double pts, float length) {
        buffer = data;
        bufferPTS = pts;
        time_length = length;
    }

    ~AudioFrame() {
        delete buffer;
    }

    QByteArray * buffer;
    double bufferPTS;
    float time_length;
};

#endif // AUDIO_FRAME_H
