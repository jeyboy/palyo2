#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "stream.h"
#include <QAudioOutput>

class AudioOutputStream : public Stream {
public:
    AudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority = InheritPriority);

protected:
    void routine();
private:
    QAudioOutput * soundOutput;
    QIODevice * audioIO;

    QByteArray audioBuffer;
    QList<int> audioBufferSizes;
};

#endif // AUDIO_OUTPUT_STREAM_H
