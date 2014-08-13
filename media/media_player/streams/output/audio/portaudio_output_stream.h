#ifndef PORTAUDIO_OUTPUT_STREAM_H
#define PORTAUDIO_OUTPUT_STREAM_H

#include "media/media_player/streams/stream.h"
#include <portaudio.h>

class PortAudioOutputStream : public Stream {
public:
    PortAudioOutputStream(QObject * parent, int bytesPerSecond, QAudioFormat & format, Priority priority = InheritPriority);
    ~PortAudioOutputStream();

    void addBuffer(QByteArray & frame);
protected:
    bool init();
    bool open();
    bool close();

    int paSampleFormat();
    void routine();
private:
    int bytes_per_sec;

    bool initialized;

    PaStreamParameters *outputParameters;
    PaStream *stream;
    double outputLatency;

    QList<QByteArray> audioBuffers;
    QAudioFormat * format;
};

#endif // PORTAUDIO_OUTPUT_STREAM_H
