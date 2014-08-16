#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "media/media_player/streams/stream.h"
#include <QAudioOutput>

class AudioOutputStream : public Stream {
    Q_OBJECT
public:
    AudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority = InheritPriority);
    ~AudioOutputStream();

    void addBuffer(QByteArray * frame);
    inline bool isFinished() { return audioBuffers.isEmpty(); }
protected slots:
    void iteration();

protected:
    void routine();
private:
    double millisPeriod;

    QAudioOutput * soundOutput;
    QIODevice * audioIO;

    QList<QByteArray *> audioBuffers;
};

#endif // AUDIO_OUTPUT_STREAM_H
