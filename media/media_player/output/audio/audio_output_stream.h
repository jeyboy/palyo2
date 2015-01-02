#ifndef AUDIO_OUTPUT_STREAM_H
#define AUDIO_OUTPUT_STREAM_H

#include "media/media_player/streams/decoders/audio_stream.h"
#include "media/media_player/utils/audio/qt_audio_device.h"

#include <QAudioOutput>
#include <QIODevice>

class AudioOutputStream : public AudioStream, public QIODevice {
public:
    static availableDevices(QList<AudioDevice> * input = 0, QList<AudioDevice> * output = 0) {
        //        QAudioFormat preferredFormat() const;

        if (input)
            foreach(QAudioDeviceInfo info, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
                input -> append(QtAudioDevice(info, QAudioDeviceInfo::defaultInputDevice() == info));

        if (output)
            foreach(QAudioDeviceInfo info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
                output -> append(QtAudioDevice(info, QAudioDeviceInfo::defaultOutputDevice() == info));
    }

    AudioOutputStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority = InheritPriority);
    virtual ~AudioOutputStream();

    uint getVolume() const;
    void setVolume(uint val);

    bool deviceInAction();

    void suspendStream();
    void resumeStream();
protected:
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);

    QAudioOutput * output;
};

#endif // AUDIO_OUTPUT_STREAM_H
