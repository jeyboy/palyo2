#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "stream.h"
#include <QAudioOutput>

#define AUDIO_BUFFERING 8
#define MAX_AUDIO_DATA_PENDING 50000 //
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // it disappeared from avcodec.h

class AudioOutputStream : public Stream {
public:
    AudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority = InheritPriority);
    ~AudioOutputStream();

    void addBuffer(QByteArray frame);
protected:
    void routine();
private:
    QAudioOutput * soundOutput;
    QIODevice * audioIO;

    QList<QByteArray> audioBuffers;
    uint buffersLength;
};

#endif // AUDIO_OUTPUT_STREAM_H
