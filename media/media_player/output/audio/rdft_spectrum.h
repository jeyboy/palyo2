#ifndef RDFT_SPECTRUM_H
#define RDFT_SPECTRUM_H

#define SAMPLE_ARRAY_SIZE (8 * 65536)

#include "media/media_player/utils/media_player_utils.h"
#include <QList>
#include <QBitmap>

//Real Discrete Fourier Transform

class RDFTSpectrum {
public:
    RDFTSpectrum(int row_height = 128, channels_count = 2) {
        height = row_height;
        nb_display_channels = channels_count;
        for (rdft_bits = 1; (1 << rdft_bits) < 2 * height; rdft_bits++);
        nb_freq = 1 << (rdft_bits - 1);
        nb_freq_rel = 1 / sqrt(nb_freq);

        rdft = av_rdft_init(rdft_bits, DFT_R2C);
        rdft_data = av_malloc(channels_count * 2 * nb_freq * sizeof(*rdft_data)); // 4
    }

    ~Spectrum() {
        av_rdft_end(rdft);
        av_free(rdft_data);
    }

    // not tested //
    QImage * compute(int16_t * sample_array) {
        QImage * image = new QImage(1, height, QImage::Format_RGB888);

        {
            FFTSample *data[nb_display_channels];
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

            for (y = 0; y < height; y++) {
                int a = sqrt(nb_freq_rel * sqrt(data[0][2 * y + 0] * data[0][2 * y + 0] + data[0][2 * y + 1] * data[0][2 * y + 1]));
                int b = (nb_display_channels == 2 ) ? sqrt(nb_freq_rel * sqrt(data[1][2 * y + 0] * data[1][2 * y + 0]
                       + data[1][2 * y + 1] * data[1][2 * y + 1])) : a;

                a = FFMIN(a, 255);
                b = FFMIN(b, 255);
                image -> setPixel(0, height - y, qRgb(a, b, (a + b) / 2));
            }
        }


        for (rdft_bits = 1; (1 << rdft_bits) < 2 * s->height; rdft_bits++)
        //        ;
        //    nb_freq = 1 << (rdft_bits - 1);

        //    /* compute display index : center on currently output samples */
        //    channels = s->audio_tgt.channels;
        //    nb_display_channels = channels;
        //    if (!s->paused) {
        //        int data_used= s->show_mode == SHOW_MODE_WAVES ? s->width : (2*nb_freq);
        //        n = 2 * channels;
        //        delay = s->audio_write_buf_size;
        //        delay /= n;

        //        /* to be more precise, we take into account the time spent since
        //           the last buffer computation */
        //        if (audio_callback_time) {
        //            time_diff = av_gettime_relative() - audio_callback_time;
        //            delay -= (time_diff * s->audio_tgt.freq) / 1000000;
        //        }

        //        delay += 2 * data_used;
        //        if (delay < data_used)
        //            delay = data_used;

        //        i_start= x = compute_mod(s->sample_array_index - delay * channels, SAMPLE_ARRAY_SIZE);
        //        if (s->show_mode == SHOW_MODE_WAVES) {
        //            h = INT_MIN;
        //            for (i = 0; i < 1000; i += channels) {
        //                int idx = (SAMPLE_ARRAY_SIZE + x - i) % SAMPLE_ARRAY_SIZE;
        //                int a = s->sample_array[idx];
        //                int b = s->sample_array[(idx + 4 * channels) % SAMPLE_ARRAY_SIZE];
        //                int c = s->sample_array[(idx + 5 * channels) % SAMPLE_ARRAY_SIZE];
        //                int d = s->sample_array[(idx + 9 * channels) % SAMPLE_ARRAY_SIZE];
        //                int score = a - d;
        //                if (h < score && (b ^ c) < 0) {
        //                    h = score;
        //                    i_start = idx;
        //                }
        //            }
        //        }

        //        s->last_i_start = i_start;
        //    } else {
        //        i_start = s->last_i_start;
        //    }

        //    bgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
        //    if (s->show_mode == SHOW_MODE_WAVES) {
        //        fill_rectangle(screen,
        //                       s->xleft, s->ytop, s->width, s->height,
        //                       bgcolor, 0);

        //        fgcolor = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);

        //        /* total height for one channel */
        //        h = s->height / nb_display_channels;
        //        /* graph height / 2 */
        //        h2 = (h * 9) / 20;
        //        for (ch = 0; ch < nb_display_channels; ch++) {
        //            i = i_start + ch;
        //            y1 = s->ytop + ch * h + (h / 2); /* position of center line */
        //            for (x = 0; x < s->width; x++) {
        //                y = (s->sample_array[i] * h2) >> 15;
        //                if (y < 0) {
        //                    y = -y;
        //                    ys = y1 - y;
        //                } else {
        //                    ys = y1;
        //                }
        //                fill_rectangle(screen,
        //                               s->xleft + x, ys, 1, y,
        //                               fgcolor, 0);
        //                i += channels;
        //                if (i >= SAMPLE_ARRAY_SIZE)
        //                    i -= SAMPLE_ARRAY_SIZE;
        //            }
        //        }

        //        fgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);

        //        for (ch = 1; ch < nb_display_channels; ch++) {
        //            y = s->ytop + ch * h;
        //            fill_rectangle(screen,
        //                           s->xleft, y, s->width, 1,
        //                           fgcolor, 0);
        //        }
        //        SDL_UpdateRect(screen, s->xleft, s->ytop, s->width, s->height);
        //    } else {
        //        nb_display_channels= FFMIN(nb_display_channels, 2);
        //        if (rdft_bits != s->rdft_bits) {
        //            av_rdft_end(s->rdft);
        //            av_free(s->rdft_data);
        //            s->rdft = av_rdft_init(rdft_bits, DFT_R2C);
        //            s->rdft_bits = rdft_bits;
        //            s->rdft_data = av_malloc(4 * nb_freq * sizeof(*s->rdft_data));
        //        }
        //        {
        //            FFTSample *data[2];
        //            for (ch = 0; ch < nb_display_channels; ch++) {
        //                data[ch] = s->rdft_data + 2 * nb_freq * ch;
        //                i = i_start + ch;
        //                for (x = 0; x < 2 * nb_freq; x++) {
        //                    double w = (x-nb_freq) * (1.0 / nb_freq);
        //                    data[ch][x] = s->sample_array[i] * (1.0 - w * w);
        //                    i += channels;
        //                    if (i >= SAMPLE_ARRAY_SIZE)
        //                        i -= SAMPLE_ARRAY_SIZE;
        //                }
        //                av_rdft_calc(s->rdft, data[ch]);
        //            }
        //            /* Least efficient way to do this, we should of course
        //             * directly access it but it is more than fast enough. */
        //            for (y = 0; y < s->height; y++) {
        //                double w = 1 / sqrt(nb_freq);
        //                int a = sqrt(w * sqrt(data[0][2 * y + 0] * data[0][2 * y + 0] + data[0][2 * y + 1] * data[0][2 * y + 1]));
        //                int b = (nb_display_channels == 2 ) ? sqrt(w * sqrt(data[1][2 * y + 0] * data[1][2 * y + 0]
        //                       + data[1][2 * y + 1] * data[1][2 * y + 1])) : a;
        //                a = FFMIN(a, 255);
        //                b = FFMIN(b, 255);
        //                fgcolor = SDL_MapRGB(screen->format, a, b, (a + b) / 2);

        //                fill_rectangle(screen,
        //                            s->xpos, s->height-y, 1, 1,
        //                            fgcolor, 0);
        //            }
        //        }
        //        SDL_UpdateRect(screen, s->xpos, s->ytop, 1, s->height);
        //        if (!s->paused)
        //            s->xpos++;
        //        if (s->xpos >= s->width)
        //            s->xpos= s->xleft;
        //    }


        return image;
    }


private:
    RDFTContext * rdft;
    int rdft_bits, nb_display_channels;
    FFTSample * rdft_data;

    int rdft_bits, nb_freq, height;
    double nb_freq_rel;
};

#endif // RDFT_SPECTRUM_H
