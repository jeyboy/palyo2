#include "audio_resampler.h"
#include <QDebug>

AudioResampler::AudioResampler() {
    ready = false;
    resampleContext = 0;
    settings = 0;
}

AudioResampler::~AudioResampler() {
    close();
}

bool AudioResampler::init(
        AVSampleFormat sampleFormatIn,
        AVSampleFormat sampleFormatOut,
        int sampleRateIn,
        int sampleRateOut,
        int channelLayoutIn,
        int channelLayoutOut
        ) {
    close();

    settings = new ResampleSettings(sampleFormatIn, sampleFormatOut, sampleRateIn, sampleRateOut, channelLayoutIn, channelLayoutOut);

    resampleContext = settings -> createContext();
    if (swr_init(resampleContext) < 0) {
        qDebug() << "SWR_INIT ERROR";
        return (ready = false);
    }

    return true;
}

bool AudioResampler::proceed(AVFrame * frame, QByteArray * data) {
    uint8_t ** buffer = settings -> outputBuffer(resampleContext, frame);
    int samples_output = swr_convert(
                    resampleContext,
                    buffer,
                    settings -> max_nb_samples_out,
                    (const uint8_t**)frame -> extended_data,
                    frame -> nb_samples
                );

    if (samples_output > -1) {
        int length = settings -> calcBufferSize(samples_output);
        data -> append((const char*)buffer[0], length);
        return true;
    } else qDebug() << "RESAMPLE ERROR";
    return false;
}

bool AudioResampler::proceed(AVFrame * frame, char * data, int & len) {
    uint8_t ** buffer = settings -> outputBuffer(resampleContext, frame);
    int samples_output = swr_convert(
                    resampleContext,
                    buffer,
                    settings -> max_nb_samples_out,
                    (const uint8_t**)frame -> extended_data,
                    frame -> nb_samples
                );

    if (samples_output > -1) {
        memcpy(data, (char*)*buffer, (len = settings -> calcBufferSize(samples_output)));
        return true;
    } else qDebug() << "RESAMPLE ERROR";

    len = 0;
    return false;
}

void AudioResampler::close() {
    if (resampleContext)
        swr_close(resampleContext);
    resampleContext = 0;

    delete settings;
    settings = 0;
}
