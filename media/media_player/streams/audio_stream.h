#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "media_stream.h"
#include "audio_output_stream.h"

#include <QAudioFormat>

#define AUDIO_BUFFERING 8
#define MAX_AUDIO_DATA_PENDING 50000
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // it disappeared from avcodec.h

class AudioStream : public MediaStream {
public:
    AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~AudioStream();

    bool decodeFrame(unsigned char * bytes, int size);
    void stop();

    void suspendOutput();
    void resumeOutput();

protected:
    void routine();
    void fillFormat(QAudioFormat & format);

private:
    void resampleInit();
    AVSampleFormat compatibleCodecType(AVCodec * codec);

    uint8_t * mResampleBuffer;
    SwrContext* resampleContext;

    AudioOutputStream * outputStream;
};

#endif // AUDIO_STREAM_H
