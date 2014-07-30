#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "stream.h"
#include <QAudioFormat>

class AudioStream : public Stream {
public:
    AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    void fillFormat(QAudioFormat & format);

private:
    AVSampleFormat compatibleCodecType(AVCodec *codec);
};

#endif // AUDIO_STREAM_H
