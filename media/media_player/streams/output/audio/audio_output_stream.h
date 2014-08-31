#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "media/media_player/streams/base/stream.h"
#include <QAudioOutput>

class AudioOutputStream : public Stream {
public:
    AudioOutputStream(void * decoderStream, QAudioFormat & format);
    ~AudioOutputStream();

    void playBuffer(QByteArray * frame);
protected:
    void routine();
private:
    QAudioOutput * soundOutput;
    QIODevice * audioIO;
    void * stream;
};

#endif // AUDIO_OUTPUT_STREAM_H
