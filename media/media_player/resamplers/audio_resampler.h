#ifndef AUDIO_RESAMPLER_H
#define AUDIO_RESAMPLER_H

#include "media/media_player/utils/media_player_utils.h"
#include <QDebug>

struct ResampleSettings {
    ResampleSettings(
            AVSampleFormat & sampleFormatIn,
            AVSampleFormat & sampleFormatOut,
            int sampleRateIn,
            int sampleRateOut,
            int channelLayoutIn,
            int channelLayoutOut) {

        this -> sampleFormatIn = sampleFormatIn;
        this -> sampleFormatOut = sampleFormatOut;
        this -> sampleRateIn = sampleRateIn;
        this -> sampleRateOut = sampleRateOut;
        this -> channelLayoutIn = channelLayoutIn;
        this -> channelLayoutOut = channelLayoutOut;
        sampleNumberIn = 1024;

        channelCountIn = av_get_channel_layout_nb_channels(channelLayoutIn);
        channelCountOut = av_get_channel_layout_nb_channels(channelLayoutOut);
        data = 0;
    }

    ~ResampleSettings() {
        clearData();
    }

    AVSampleFormat sampleFormatIn;
    AVSampleFormat sampleFormatOut;
    int sampleRateIn;
    int sampleRateOut;
    int channelLayoutIn;
    int channelLayoutOut;
    int sampleNumberIn;

    int channelCountIn;
    int channelCountOut;

    int max_nb_samples_out;
    int linesize;
    uint8_t ** data;

    SwrContext * createContext() {
        SwrContext * resampleContext = swr_alloc();
        av_opt_set_int(resampleContext, "in_channel_layout", channelLayoutIn, 0);
        av_opt_set_int(resampleContext, "out_channel_layout", channelLayoutOut, 0);
        av_opt_set_int(resampleContext, "in_sample_rate", sampleRateIn, 0);
        av_opt_set_int(resampleContext, "out_sample_rate", sampleRateOut, 0);
        av_opt_set_sample_fmt(resampleContext, "in_sample_fmt", sampleFormatIn, 0);
        av_opt_set_sample_fmt(resampleContext, "out_sample_fmt", sampleFormatOut, 0);

        initBuffer();

        return resampleContext;
    }

    void clearData() {
        if (data)
            av_freep(&data[0]);
        av_freep(&data);
    }

    void initBuffer() {
        max_nb_samples_out = 0;

//        ret = av_samples_alloc_array_and_samples(&data, &linesize, channelCountOut, max_nb_samples_out, sampleFormatOut, 0);
    }

    uint8_t ** outputBuffer(SwrContext * ctx, AVFrame * frame) {
        int nb_samples_out = calcSamplesNumber(ctx, frame);

        if (nb_samples_out > max_nb_samples_out) {
            clearData();
            int ret = av_samples_alloc_array_and_samples(&data, &linesize, channelCountOut,  nb_samples_out, sampleFormatOut, 0);//1);
            if (ret < 0)
               return 0;
            max_nb_samples_out = nb_samples_out;
        }

        return data;
    }

    int calcSamplesNumber(SwrContext * swr_ctx) {
        return av_rescale_rnd(
            swr_get_delay(swr_ctx, sampleRateIn) + sampleNumberIn,
            sampleRateOut,
            sampleRateIn,
            AV_ROUND_UP
        );
    }

    int calcSamplesNumber(SwrContext * swr_ctx, AVFrame * frame) {
        return av_rescale_rnd(
            swr_get_delay(swr_ctx, frame -> sample_rate) + frame -> nb_samples,
            sampleRateOut,
            frame -> sample_rate,
            AV_ROUND_UP
        );
    }

    int calcBufferSize(int convertResult) {
        return av_samples_get_buffer_size(&linesize, channelCountOut, convertResult, sampleFormatOut, 0);//1);
    }
};


class AudioResampler {
public:
    AudioResampler();
    ~AudioResampler();
    bool init(
        AVSampleFormat sampleFormatIn,
        AVSampleFormat sampleFormatOut,
        int sampleRateIn,
        int sampleRateOut,
        int channelLayoutIn,
        int channelLayoutOut
    );

    bool proceed(AVFrame * frame, QByteArray & result);
private:
    void close();

    bool ready;

    SwrContext * resampleContext;
    ResampleSettings * settings;
};

#endif // AUDIO_RESAMPLER_H
