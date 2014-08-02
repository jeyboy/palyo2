#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "stream.h"
#include <QAudioOutput>

class AudioOutputStream : public Stream {
public:
    AudioOutputStream(QObject * parent, QAudioFormat & format, QMutex * mutex, Priority priority = InheritPriority);
    ~AudioOutputStream();
protected:
    void routine();
private:
    QAudioOutput * soundOutput;
    QIODevice * audioIO;

    QList<QByteArray> audioBuffers;
};

#endif // AUDIO_OUTPUT_STREAM_H
