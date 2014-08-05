#ifndef PORTAUDIO_OUTPUT_STREAM_H
#define PORTAUDIO_OUTPUT_STREAM_H

#include "stream.h"
#include <portaudio.h>

class PortAudioOutputStream : public Stream {
public:
    PortAudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority = InheritPriority);
    ~PortAudioOutputStream();

    void addBuffer(QByteArray & frame);
protected:
    bool init();
    bool open();
    bool close();

    int paSampleFormat();
    void routine();
private:
    bool initialized;

    PaStreamParameters *outputParameters;
    PaStream *stream;
    double outputLatency;

    QList<QByteArray> audioBuffers;
    uint buffersLength;
    QAudioFormat * format;
};

#endif // PORTAUDIO_OUTPUT_STREAM_H
