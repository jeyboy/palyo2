#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "media/media_player/streams/decoders/audio_stream.h"

//#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QIODevice>
#include <QAudioFormat>

class AudioOutputStream : public QIODevice, public AudioStream {
public:
    AudioOutputStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority = InheritPriority);
    virtual ~AudioOutputStream();

    uint getVolume() const;
    void setVolume(uint val);

    bool deviceInAction();

    void suspendStream();
    void resumeStream();
protected:
    void fillFormat(QAudioFormat & format);
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);

    QAudioOutput * output;
};

#endif // AUDIO_OUTPUT_STREAM_H
