#ifndef PORTAUDIO_OUTPUT_STREAM_H
#define PORTAUDIO_OUTPUT_STREAM_H

#include <QAudioFormat>
#include <QList>
#include <portaudio.h>

class PortAudioOutputStream {
public:
    PortAudioOutputStream(void * dStream, int bytesPerSecond, QAudioFormat & format);
    ~PortAudioOutputStream();

//    void addBuffer(QByteArray & frame);
protected:
    bool init();
    bool open(void * dStream);
    bool close();

    int paSampleFormat();
//    void routine();
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
