#ifndef PORTAUDIO_OUTPUT_STREAM_H
#define PORTAUDIO_OUTPUT_STREAM_H

#include "media/media_player/streams/decoders/audio_stream.h"
#include <QAudioFormat>
#include <portaudio.h>

class PortAudioOutputStream : public AudioStream {
public:
    PortAudioOutputStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority = InheritPriority);
    virtual ~PortAudioOutputStream();

    uint getVolume() const;
    void setVolume(uint val);

    bool deviceInAction();

    void suspendStream();
    void resumeStream();

    inline uint bytesPerBuffer(uint framesPerBuffer) { return format.bytesForFrames(framesPerBuffer); }

////    void addBuffer(QByteArray & frame);
protected:
    bool init();
    bool open();
    bool close();

    int paSampleFormat();

private:
//    int bytes_per_sec;
    bool initialized;

    PaStreamParameters *outputParameters;
    PaStream *stream;
    double outputLatency;
};

#endif // PORTAUDIO_OUTPUT_STREAM_H
