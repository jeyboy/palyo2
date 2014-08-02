#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "media_stream.h"
#include "audio_output_stream.h"

#include <QAudioFormat>

class AudioStream : public MediaStream {
public:
    AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~AudioStream();

    void decode(unsigned char * bytes, int size);
    void stop();

    void suspendOutput();
    void resumeOutput();

protected:
    void routine();
    void fillFormat(QAudioFormat & format);

private:
    void resampleInit(AVSampleFormat sampleFormat);
    AVSampleFormat compatibleCodecType(AVCodec * codec);

    bool resampleRequire;

    uint8_t * resampleBuffer;
    SwrContext* resampleContext;

    AudioOutputStream * outputStream;
};

#endif // AUDIO_STREAM_H
