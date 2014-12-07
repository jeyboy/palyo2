#ifndef FFT_SPECTRUM_H
#define FFT_SPECTRUM_H

#include "media/media_player/utils/media_player_utils.h"
#include <QList>

//Fast Fourier Transform

class FFTSpectrum {
public:
    FFTSpectrum(int size = 1024, channels_count = 2) {
        nb_display_channels = channels_count;
        for (fft_bits = 1; (1 << fft_bits) < 2 * size; fft_bits++);
        nb_freq = 1 << (fft_bits - 1);
        nb_freq_rel = 1 / sqrt(nb_freq);

        fft = av_fft_init(fft_bits, 0);  // 0 - forward transform; 1 - inverse
        fft_data = av_malloc(channels_count * nb_freq * sizeof(*fft_data));
    }

    ~FFTSpectrum() {
        av_fft_end(fft);
        av_free(fft_data);
    }

    QVector<int> compute(int16_t * sample_array) {



        for (x = 0; x < 2 * nb_freq; x++) {
            double w = (x - nb_freq) * (1.0 / nb_freq);

            data[ch][x] = sample_array[i] * (1.0 - w * w);
            i += channels;
            if (i >= SAMPLE_ARRAY_SIZE)
                i -= SAMPLE_ARRAY_SIZE;
        }





        {
            FFTSample *data[2];
            for (ch = 0; ch < nb_display_channels; ch++) {
                data[ch] = rdft_data + 2 * nb_freq * ch;
                i = i_start + ch;
                for (x = 0; x < 2 * nb_freq; x++) {
                    double w = (x - nb_freq) * (1.0 / nb_freq);
                    data[ch][x] = sample_array[i] * (1.0 - w * w);
                    i += channels;
                    if (i >= SAMPLE_ARRAY_SIZE)
                        i -= SAMPLE_ARRAY_SIZE;
                }
                av_rdft_calc(rdft, data[ch]);
            }

            /* Least efficient way to do this, we should of course
             * directly access it but it is more than fast enough. */

            for (y = 0; y < s->height; y++) {
                int a = sqrt(nb_freq_rel * sqrt(data[0][2 * y + 0] * data[0][2 * y + 0] + data[0][2 * y + 1] * data[0][2 * y + 1]));
                int b = (nb_display_channels == 2 ) ? sqrt(nb_freq_rel * sqrt(data[1][2 * y + 0] * data[1][2 * y + 0]
                       + data[1][2 * y + 1] * data[1][2 * y + 1])) : a;

                a = FFMIN(a, 255);
                b = FFMIN(b, 255);

                fgcolor = SDL_MapRGB(screen->format, a, b, (a + b) / 2);

                fill_rectangle(screen,
                            s->xpos, s->height-y, 1, 1,
                            fgcolor, 0);
            }
        }








//        int layerLimit = 1024, gLimit = layerLimit * channelsCount;
//        int spectrumMultiplicity = Settings::instance() -> getSpectrumMultiplier();
//        int workSpectrumBandsCount = getCalcSpectrumBandsCount();
//        float fft[gLimit];
//        BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL | BASS_DATA_FFT_REMOVEDC);

//        QList<QVector<int> > res;
//        QVector<float> peaks;
//        int b0 = 0, x, y, z, peakNum;

//        for (x = 0; x < channelsCount; x++)
//            res.append(QVector<int>());

//        for (x = 0; x < workSpectrumBandsCount; x++) {
//            peaks.clear();
//            peaks.fill(0, channelsCount);

//            int b1 = pow(2, x * 10.0 / (workSpectrumBandsCount - 1)) * channelsCount;
//            if (b1 - channelsCount <= b0) b1 = b0 + channelsCount * 2; // make sure it uses at least 2 FFT bin
//            if (b1 > gLimit - 1) b1 = gLimit - 1; // prevent index overflow

//            for (; b0 < b1; b0++) {
//                peakNum = b0 % channelsCount;
//                if (peaks[peakNum] < fft[b0])
//                    peaks[peakNum] = fft[b0];
//            }

//            for (z = 0; z < channelsCount; z++) {
//                y = sqrt(peaks[z]) * spectrumMultiplicity * spectrumHeight + defaultSpectrumLevel; // 4 // scale it (sqrt to make low values more visible)
//                if (y > spectrumHeight) y = spectrumHeight; // cap it
//                if (y < defaultSpectrumLevel) y = defaultSpectrumLevel; // cap it

//                res[z].append(y);
//            }
//        }

//        return res;
    }


private:
    FFTContext * fft;
    int fft_bits, nb_display_channels;
    FFTComplex * fft_data;

    int fft_bits, nb_freq;
};

#endif // FFT_SPECTRUM_H
