#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "media_stream.h"
#include "audio_output_stream.h"
#include "portaudio_output_stream.h"

#include <QAudioFormat>

//struct AudioInfo {
//    int channels;
//    int sample_rate;
//    int sample_size;
//    int nb_samples;
//};

class AudioStream : public MediaStream {
public:
    AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~AudioStream();

    void stop();

    void suspendOutput();
    void resumeOutput();

protected:
    void routine();
    void fillFormat(QAudioFormat & format);
    double calcPts(AVPacket * packet);

private:
    void resampleInit(AVSampleFormat sampleFormat);
    AVSampleFormat compatibleCodecType(AVCodec * codec);
    int selectSampleRate(AVCodec *codec);
    int selectChannelLayout(AVCodec *codec);

    bool resampleRequire;
    int resample_nb_samples;

    uint8_t * resampleBuffer;
    SwrContext* resampleContext;

//    AudioOutputStream * outputStream;
    PortAudioOutputStream * outputStream;
};

#endif // AUDIO_STREAM_H
