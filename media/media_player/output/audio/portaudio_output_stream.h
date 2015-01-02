#ifndef PORTAUDIO_OUTPUT_STREAM_H
#define PORTAUDIO_OUTPUT_STREAM_H

#include "media/media_player/utils/audio/port_audio_device.h"
#include "media/media_player/streams/decoders/audio_stream.h"
#include <QAudioFormat>

class PortAudioOutputStream : public AudioStream {
public:
    static availableDevices(QList<AudioDevice> * input = 0, QList<AudioDevice> * output = 0) {
        int numDevices = Pa_GetDeviceCount();

        for (int i = 0 ; i < numDevices ; ++i) {
            const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
            if (deviceInfo) {
                if (deviceInfo -> maxInputChannels > 0 && input)
                    input -> append(PortAudioDevice(deviceInfo, i == Pa_GetDefaultInputDevice()));

                if (deviceInfo -> maxOutputChannels > 0 && output)
                    output -> append(PortAudioDevice(deviceInfo, i == Pa_GetDefaultOutputDevice()));
            }
        }
    }

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
