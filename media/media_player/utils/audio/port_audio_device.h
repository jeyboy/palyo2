#ifndef PORT_AUDIO_DEVICE
#define PORT_AUDIO_DEVICE

#include "audio_device.h"
#include <portaudio.h>

struct PortAudioDevice : public AudioDevice {
//    QString toString(QAudioFormat::SampleType sampleType) {
//        switch (sampleType) {
//            case QAudioFormat::SignedInt:
//                return "SignedInt";
//            case QAudioFormat::UnSignedInt:
//                return "UnSignedInt";
//            case QAudioFormat::Float:
//                return "Float";
//            default: return "Unknown";
//        }
//    }

//    QString toString(QAudioFormat::Endian endian) {
//        switch (endian) {
//            case QAudioFormat::LittleEndian:
//                return "LittleEndian";
//            case QAudioFormat::BigEndian:
//                return "BigEndian";
//            default: return "Unknown";
//        }
//    }

    PortAudioDevice(PaDeviceInfo * deviceInfo, bool isDefault) {
        is_default = isDefault;

        const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo -> hostApi);
        QString name = QString(hostApiInfo -> name) + ": " + QString::fromLocal8Bit(deviceInfo -> name);
        qDebug("audio device %d: %s", i, name.toUtf8().constData());
        qDebug("max in/out channels: %d/%d", deviceInfo -> maxInputChannels, deviceInfo -> maxOutputChannels);



//        name = info.deviceName();
//        supportedChannelCounts = info.supportedChannelCounts();
//        supportedCodecs = info.supportedCodecs();
//        supportedSampleRates = info.supportedSampleRates();
//        supportedSampleSizes = info.supportedSampleSizes();

//        foreach (QAudioFormat::SampleType type, info.supportedSampleTypes())
//            supportedSampleTypes.append(toString(type));

//        foreach (QAudioFormat::Endian endian, info.supportedByteOrders())
//            supportedByteOrders.append(toString(endian));
    }
};

#endif // PORT_AUDIO_DEVICE

