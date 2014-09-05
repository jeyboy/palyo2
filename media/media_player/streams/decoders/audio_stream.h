#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "media/media_player/resamplers/audio_resampler.h"
#include "media/media_player/streams/base/media_stream.h"
#include "media/media_player/utils/audio_frame.h"

#include <QIODevice>
#include <QAudioFormat>

class AudioStream : public QIODevice, public MediaStream {
public:
    AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);
    ~AudioStream();

    bool isBlocked();

    void suspend();
    void resume();
    void flushData();

protected:
    void routine();
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

//    void sync(double delay, char *data, int & len, qint64 maxlen);
    void fillFormat(QAudioFormat & format);
    double calcPts(AVPacket * packet);
    int bytesPerSecond();

private:
    int defaultChannelLayout;

    bool resampleRequire;
    AudioResampler * resampler;

    QAudioOutput * output;
    QList<AudioFrame *> frames;
};

#endif // AUDIO_STREAM_H


//static int audio_decode_frame(VideoState *is)
//{
//    AVPacket *pkt_temp = &is->audio_pkt_temp;
//    AVPacket *pkt = &is->audio_pkt;
//    AVCodecContext *dec = is->audio_st->codec;
//    int len1, data_size, resampled_data_size;
//    int64_t dec_channel_layout;
//    int got_frame;
//    av_unused double audio_clock0;
//    int wanted_nb_samples;
//    AVRational tb;
//    int ret;
//    int reconfigure;

//    for (;;) {
//        /* NOTE: the audio packet can contain several frames */
//        while (pkt_temp->stream_index != -1 || is->audio_buf_frames_pending) {
//            if (!is->frame) {
//                if (!(is->frame = av_frame_alloc()))
//                    return AVERROR(ENOMEM);
//            } else {
//                av_frame_unref(is->frame);
//            }

//            if (is->audioq.serial != is->audio_pkt_temp_serial)
//                break;

//            if (is->paused)
//                return -1;

//            if (!is->audio_buf_frames_pending) {
//                len1 = avcodec_decode_audio4(dec, is->frame, &got_frame, pkt_temp);
//                if (len1 < 0) {
//                    /* if error, we skip the frame */
//                    pkt_temp->size = 0;
//                    break;
//                }

//                pkt_temp->dts =
//                pkt_temp->pts = AV_NOPTS_VALUE;
//                pkt_temp->data += len1;
//                pkt_temp->size -= len1;
//                if (pkt_temp->data && pkt_temp->size <= 0 || !pkt_temp->data && !got_frame)
//                    pkt_temp->stream_index = -1;
//                if (!pkt_temp->data && !got_frame)
//                    is->audio_finished = is->audio_pkt_temp_serial;

//                if (!got_frame)
//                    continue;

//                tb = (AVRational){1, is->frame->sample_rate};
//                if (is->frame->pts != AV_NOPTS_VALUE)
//                    is->frame->pts = av_rescale_q(is->frame->pts, dec->time_base, tb);
//                else if (is->frame->pkt_pts != AV_NOPTS_VALUE)
//                    is->frame->pts = av_rescale_q(is->frame->pkt_pts, is->audio_st->time_base, tb);
//                else if (is->audio_frame_next_pts != AV_NOPTS_VALUE)
//#if CONFIG_AVFILTER
//                    is->frame->pts = av_rescale_q(is->audio_frame_next_pts, (AVRational){1, is->audio_filter_src.freq}, tb);
//#else
//                    is->frame->pts = av_rescale_q(is->audio_frame_next_pts, (AVRational){1, is->audio_src.freq}, tb);
//#endif

//                if (is->frame->pts != AV_NOPTS_VALUE)
//                    is->audio_frame_next_pts = is->frame->pts + is->frame->nb_samples;

//#if CONFIG_AVFILTER
//                dec_channel_layout = get_valid_channel_layout(is->frame->channel_layout, av_frame_get_channels(is->frame));

//                reconfigure =
//                    cmp_audio_fmts(is->audio_filter_src.fmt, is->audio_filter_src.channels,
//                                   is->frame->format, av_frame_get_channels(is->frame))    ||
//                    is->audio_filter_src.channel_layout != dec_channel_layout ||
//                    is->audio_filter_src.freq           != is->frame->sample_rate ||
//                    is->audio_pkt_temp_serial           != is->audio_last_serial;

//                if (reconfigure) {
//                    char buf1[1024], buf2[1024];
//                    av_get_channel_layout_string(buf1, sizeof(buf1), -1, is->audio_filter_src.channel_layout);
//                    av_get_channel_layout_string(buf2, sizeof(buf2), -1, dec_channel_layout);
//                    av_log(NULL, AV_LOG_DEBUG,
//                           "Audio frame changed from rate:%d ch:%d fmt:%s layout:%s serial:%d to rate:%d ch:%d fmt:%s layout:%s serial:%d\n",
//                           is->audio_filter_src.freq, is->audio_filter_src.channels, av_get_sample_fmt_name(is->audio_filter_src.fmt), buf1, is->audio_last_serial,
//                           is->frame->sample_rate, av_frame_get_channels(is->frame), av_get_sample_fmt_name(is->frame->format), buf2, is->audio_pkt_temp_serial);

//                    is->audio_filter_src.fmt            = is->frame->format;
//                    is->audio_filter_src.channels       = av_frame_get_channels(is->frame);
//                    is->audio_filter_src.channel_layout = dec_channel_layout;
//                    is->audio_filter_src.freq           = is->frame->sample_rate;
//                    is->audio_last_serial               = is->audio_pkt_temp_serial;

//                    if ((ret = configure_audio_filters(is, afilters, 1)) < 0)
//                        return ret;
//                }

//                if ((ret = av_buffersrc_add_frame(is->in_audio_filter, is->frame)) < 0)
//                    return ret;
//#endif
//            }
//#if CONFIG_AVFILTER
//            if ((ret = av_buffersink_get_frame_flags(is->out_audio_filter, is->frame, 0)) < 0) {
//                if (ret == AVERROR(EAGAIN)) {
//                    is->audio_buf_frames_pending = 0;
//                    continue;
//                }
//                if (ret == AVERROR_EOF)
//                    is->audio_finished = is->audio_pkt_temp_serial;
//                return ret;
//            }
//            is->audio_buf_frames_pending = 1;
//            tb = is->out_audio_filter->inputs[0]->time_base;
//#endif

//            data_size = av_samples_get_buffer_size(NULL, av_frame_get_channels(is->frame),
//                                                   is->frame->nb_samples,
//                                                   is->frame->format, 1);

//            dec_channel_layout =
//                (is->frame->channel_layout && av_frame_get_channels(is->frame) == av_get_channel_layout_nb_channels(is->frame->channel_layout)) ?
//                is->frame->channel_layout : av_get_default_channel_layout(av_frame_get_channels(is->frame));
//            wanted_nb_samples = synchronize_audio(is, is->frame->nb_samples);

//            if (is->frame->format        != is->audio_src.fmt            ||
//                dec_channel_layout       != is->audio_src.channel_layout ||
//                is->frame->sample_rate   != is->audio_src.freq           ||
//                (wanted_nb_samples       != is->frame->nb_samples && !is->swr_ctx)) {
//                swr_free(&is->swr_ctx);
//                is->swr_ctx = swr_alloc_set_opts(NULL,
//                                                 is->audio_tgt.channel_layout, is->audio_tgt.fmt, is->audio_tgt.freq,
//                                                 dec_channel_layout,           is->frame->format, is->frame->sample_rate,
//                                                 0, NULL);
//                if (!is->swr_ctx || swr_init(is->swr_ctx) < 0) {
//                    av_log(NULL, AV_LOG_ERROR,
//                           "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
//                            is->frame->sample_rate, av_get_sample_fmt_name(is->frame->format), av_frame_get_channels(is->frame),
//                            is->audio_tgt.freq, av_get_sample_fmt_name(is->audio_tgt.fmt), is->audio_tgt.channels);
//                    break;
//                }
//                is->audio_src.channel_layout = dec_channel_layout;
//                is->audio_src.channels       = av_frame_get_channels(is->frame);
//                is->audio_src.freq = is->frame->sample_rate;
//                is->audio_src.fmt = is->frame->format;
//            }

//            if (is->swr_ctx) {
//                const uint8_t **in = (const uint8_t **)is->frame->extended_data;
//                uint8_t **out = &is->audio_buf1;
//                int out_count = (int64_t)wanted_nb_samples * is->audio_tgt.freq / is->frame->sample_rate + 256;
//                int out_size  = av_samples_get_buffer_size(NULL, is->audio_tgt.channels, out_count, is->audio_tgt.fmt, 0);
//                int len2;
//                if (out_size < 0) {
//                    av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
//                    break;
//                }
//                if (wanted_nb_samples != is->frame->nb_samples) {
//                    if (swr_set_compensation(is->swr_ctx, (wanted_nb_samples - is->frame->nb_samples) * is->audio_tgt.freq / is->frame->sample_rate,
//                                                wanted_nb_samples * is->audio_tgt.freq / is->frame->sample_rate) < 0) {
//                        av_log(NULL, AV_LOG_ERROR, "swr_set_compensation() failed\n");
//                        break;
//                    }
//                }
//                av_fast_malloc(&is->audio_buf1, &is->audio_buf1_size, out_size);
//                if (!is->audio_buf1)
//                    return AVERROR(ENOMEM);
//                len2 = swr_convert(is->swr_ctx, out, out_count, in, is->frame->nb_samples);
//                if (len2 < 0) {
//                    av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
//                    break;
//                }
//                if (len2 == out_count) {
//                    av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
//                    swr_init(is->swr_ctx);
//                }
//                is->audio_buf = is->audio_buf1;
//                resampled_data_size = len2 * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
//            } else {
//                is->audio_buf = is->frame->data[0];
//                resampled_data_size = data_size;
//            }

//            audio_clock0 = is->audio_clock;
//            /* update the audio clock with the pts */
//            if (is->frame->pts != AV_NOPTS_VALUE)
//                is->audio_clock = is->frame->pts * av_q2d(tb) + (double) is->frame->nb_samples / is->frame->sample_rate;
//            else
//                is->audio_clock = NAN;
//            is->audio_clock_serial = is->audio_pkt_temp_serial;
//#ifdef DEBUG
//            {
//                static double last_clock;
//                printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
//                       is->audio_clock - last_clock,
//                       is->audio_clock, audio_clock0);
//                last_clock = is->audio_clock;
//            }
//#endif
//            return resampled_data_size;
//        }

//        /* free the current packet */
//        if (pkt->data)
//            av_free_packet(pkt);
//        memset(pkt_temp, 0, sizeof(*pkt_temp));
//        pkt_temp->stream_index = -1;

//        if (is->audioq.abort_request) {
//            return -1;
//        }

//        if (is->audioq.nb_packets == 0)
//            SDL_CondSignal(is->continue_read_thread);

//        /* read next packet */
//        if ((packet_queue_get(&is->audioq, pkt, 1, &is->audio_pkt_temp_serial)) < 0)
//            return -1;

//        if (pkt->data == flush_pkt.data) {
//            avcodec_flush_buffers(dec);
//            is->audio_buf_frames_pending = 0;
//            is->audio_frame_next_pts = AV_NOPTS_VALUE;
//            if ((is->ic->iformat->flags & (AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH | AVFMT_NO_BYTE_SEEK)) && !is->ic->iformat->read_seek)
//                is->audio_frame_next_pts = is->audio_st->start_time;
//        }

//        *pkt_temp = *pkt;
//    }
//}

///* prepare a new audio buffer */
//static void sdl_audio_callback(void *opaque, Uint8 *stream, int len)
//{
//    VideoState *is = opaque;
//    int audio_size, len1;

//    audio_callback_time = av_gettime_relative();

//    while (len > 0) {
//        if (is->audio_buf_index >= is->audio_buf_size) {
//           audio_size = audio_decode_frame(is);
//           if (audio_size < 0) {
//                /* if error, just output silence */
//               is->audio_buf      = is->silence_buf;
//               is->audio_buf_size = sizeof(is->silence_buf) / is->audio_tgt.frame_size * is->audio_tgt.frame_size;
//           } else {
//               if (is->show_mode != SHOW_MODE_VIDEO)
//                   update_sample_display(is, (int16_t *)is->audio_buf, audio_size);
//               is->audio_buf_size = audio_size;
//           }
//           is->audio_buf_index = 0;
//        }
//        len1 = is->audio_buf_size - is->audio_buf_index;
//        if (len1 > len)
//            len1 = len;
//        memcpy(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1);
//        len -= len1;
//        stream += len1;
//        is->audio_buf_index += len1;
//    }
//    is->audio_write_buf_size = is->audio_buf_size - is->audio_buf_index;
//    /* Let's assume the audio driver that is used by SDL has two periods. */
//    if (!isnan(is->audio_clock)) {
//        set_clock_at(&is->audclk, is->audio_clock - (double)(2 * is->audio_hw_buf_size + is->audio_write_buf_size) / is->audio_tgt.bytes_per_sec, is->audio_clock_serial, audio_callback_time / 1000000.0);
//        sync_clock_to_slave(&is->extclk, &is->audclk);
//    }
//}

