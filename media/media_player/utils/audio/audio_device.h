#ifndef AUDIO_DEVICE
#define AUDIO_DEVICE

#include <qstring>
#include <qstringlist>

struct AudioDevice {
    QString name;

    QStringList supportedCodecs;
    QList<int> supportedSampleRates;
    QList<int> supportedChannelCounts;
    QList<int> supportedSampleSizes;

    QStringList supportedByteOrders;
    QStringList supportedSampleTypes;

    bool is_default;
};

#endif // AUDIO_DEVICE

