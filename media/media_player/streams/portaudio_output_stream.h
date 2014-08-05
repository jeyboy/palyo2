#ifndef PORTAUDIO_OUTPUT_STREAM_H
#define PORTAUDIO_OUTPUT_STREAM_H

#include "stream.h"
#include <portaudio.h>

//class AudioOutputPortAudioPrivate : public AudioOutputPrivate
//{
//public:
//    AudioOutputPortAudioPrivate():
//        initialized(false)
//      ,outputParameters(new PaStreamParameters)
//      ,stream(0)
//    {
//        PaError err = paNoError;
//        if ((err = Pa_Initialize()) != paNoError) {
//            qWarning("Error when init portaudio: %s", Pa_GetErrorText(err));
//            available = false;
//            return;
//        }
//        initialized = true;

//        int numDevices = Pa_GetDeviceCount();
//        for (int i = 0 ; i < numDevices ; ++i) {
//            const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
//            if (deviceInfo) {
//                const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
//                QString name = QString(hostApiInfo->name) + ": " + QString::fromLocal8Bit(deviceInfo->name);
//                qDebug("audio device %d: %s", i, name.toUtf8().constData());
//                qDebug("max in/out channels: %d/%d", deviceInfo->maxInputChannels, deviceInfo->maxOutputChannels);
//            }
//        }
//        memset(outputParameters, 0, sizeof(PaStreamParameters));
//        outputParameters->device = Pa_GetDefaultOutputDevice();
//        if (outputParameters->device == paNoDevice) {
//            qWarning("PortAudio get device error!");
//            available = false;
//            return;
//        }
//        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(outputParameters->device);
//        max_channels = deviceInfo->maxOutputChannels;
//        qDebug("DEFAULT max in/out channels: %d/%d", deviceInfo->maxInputChannels, deviceInfo->maxOutputChannels);
//        qDebug("audio device: %s", QString::fromLocal8Bit(Pa_GetDeviceInfo(outputParameters->device)->name).toUtf8().constData());
//        outputParameters->hostApiSpecificStreamInfo = NULL;
//        outputParameters->suggestedLatency = Pa_GetDeviceInfo(outputParameters->device)->defaultHighOutputLatency;
//    }

//};

//} //namespace QtAV















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
