#ifndef QT_AUDIO_DEVICE
#define QT_AUDIO_DEVICE

#include "audio_device.h"
#include <qaudiodeviceinfo>

struct QtAudioDevice : public AudioDevice {
    QString toString(QAudioFormat::SampleType sampleType) {
        switch (sampleType) {
            case QAudioFormat::SignedInt:
                return "SignedInt";
            case QAudioFormat::UnSignedInt:
                return "UnSignedInt";
            case QAudioFormat::Float:
                return "Float";
            default: return "Unknown";
        }
    }

    QString toString(QAudioFormat::Endian endian) {
        switch (endian) {
            case QAudioFormat::LittleEndian:
                return "LittleEndian";
            case QAudioFormat::BigEndian:
                return "BigEndian";
            default: return "Unknown";
        }
    }

    QtAudioDevice(QAudioDeviceInfo info, bool isDefault) {
        is_default = isDefault;
        name = info.deviceName();
        supportedChannelCounts = info.supportedChannelCounts();
        supportedCodecs = info.supportedCodecs();
        supportedSampleRates = info.supportedSampleRates();
        supportedSampleSizes = info.supportedSampleSizes();

        foreach (QAudioFormat::SampleType type, info.supportedSampleTypes())
            supportedSampleTypes.append(toString(type));

        foreach (QAudioFormat::Endian endian, info.supportedByteOrders())
            supportedByteOrders.append(toString(endian));
    }
};

#endif // QT_AUDIO_DEVICE

