#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , isPlanar(false)
    , resampleRequire(false)
    , resampleContext(0) {

    isPlanar = (codec_context -> channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codec_context -> sample_fmt));
    //    std::cout << "The audio stream (and its frames) have too many channels to fit in\n"
    //              << "frame->data. Therefore, to access the audio data, you need to use\n"
    //              << "frame->extended_data to access the audio data. It's planar, so\n"
    //              << "each channel is in a different element. That is:\n"
    //              << "  frame->extended_data[0] has the data for channel 1\n"
    //              << "  frame->extended_data[1] has the data for channel 2\n"
    //              << "  etc.\n";

    bufferLimit = 500;
    QAudioFormat format;
    fillFormat(format);

    outputStream = new AudioOutputStream(this, format, priority);
//    outputStream = new PortAudioOutputStream(this, format, priority);
}

AudioStream::~AudioStream() {
    qDebug() << "Audion stream";
    outputStream -> stop();
    outputStream -> wait();

    delete [] resampleBuffer;

    if (resampleContext)
        swr_close(resampleContext);
}

void AudioStream::stop() {
    outputStream -> stop();
    MediaStream::stop();
}

void AudioStream::suspendOutput() {
    outputStream -> suspend();
}
void AudioStream::resumeOutput() {
    outputStream -> resume();
}

//void AudioStream::routine() {
//    mutex -> lock();
//    if (packets.isEmpty()) {
//        mutex -> unlock();
//        pauseRequired = finishAndPause;
//        return;
//    }

//    AVPacket * packet = packets.takeFirst();
//    mutex -> unlock();

//    int len, out_size;
//    uint8_t * med_buffer = (uint8_t*)malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);

//    while (packet -> size > 0) {
//        out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
//        len = avcodec_decode_audio3(codec_context, (int16_t*)med_buffer, &out_size, packet);

//        if (len < 0) {
//            qDebug() << "Error while decoding audio frame";
//            av_free_packet(packet);
//            return;
//        }

//        packet -> size -= len;
//        packet -> data += len;

//        QByteArray ar((const char*)med_buffer, out_size);
//        outputStream -> addBuffer(ar);

//        calcPts(packet);
//    }

//    av_free_packet(packet);
//    delete [] med_buffer;
//}

void AudioStream::routine() {
    mutex -> lock();
    if (packets.isEmpty()) {
        mutex -> unlock();
        pauseRequired = finishAndPause;
        return;
    }

    AVPacket * packet = packets.takeFirst();
    mutex -> unlock();

    int samples_output;
    int len, got_frame;

    while (packet -> size > 0) {
        avcodec_get_frame_defaults(frame);
        len = avcodec_decode_audio4(codec_context, frame, &got_frame, packet);

        if (len < 0) {
            qDebug() << "Error while decoding audio frame";
            av_free_packet(packet);
            return;
        }

        if (got_frame) {
            // Resample to S16
            if (resampleRequire) {
                qDebug() << "USE RESAMPLE";
                samples_output = swr_convert(
                                resampleContext,
                                &resampleBuffer,
                                resample_nb_samples,
                                (const uint8_t**)frame -> extended_data,
                                frame -> nb_samples
                            );

                if (samples_output > 0) {
                    // A frame has been decoded. Queue it to our buffer.

                    QByteArray ar((const char*)resampleBuffer, samples_output * 4);
                    outputStream -> addBuffer(ar);
                }
            } else {
                manualResample();



//                QByteArray ar((const char*)frame -> data[0], frame -> linesize[0]);


////                int data_size = av_samples_get_buffer_size(NULL, codec_context -> channels,
////                                                           frame -> nb_samples,
////                                                           codec_context -> sample_fmt, 1);

////                QByteArray ar((const char*)frame -> data[0], data_size);//frame -> linesize[0]);//frame -> nb_samples);
////                QByteArray ar((const char*)*frame -> extended_data, frame -> nb_samples);
//                outputStream -> addBuffer(ar);
            }

            calcPts(packet);
        } else {
            qDebug() << "Could not get audio data from this frame";
        }

        packet -> size -= len;
        packet -> data += len;
    }

    av_free_packet(packet);
}

void AudioStream::manualResample() {
    int samples_with_channels = frame -> nb_samples * codec_context -> channels;
    int samples_with_channels_half = samples_with_channels / 2;
    QByteArray temp;


    temp.resize(samples_with_channels * sizeof(float));
    float *decoded_data = (float*)temp.data();

    static const float kInt8_inv = 1.0f/128.0f;
    static const float kInt16_inv = 1.0f/32768.0f;
    static const float kInt32_inv = 1.0f/2147483648.0f;

    //TODO: hwa
    //https://code.google.com/p/lavfilters/source/browse/decoder/LAVAudio/LAVAudio.cpp

    switch (codec_context -> sample_fmt) {
        case AV_SAMPLE_FMT_U8: {
            uint8_t *data = (uint8_t*)*frame -> data;
            for (int i = 0; i < samples_with_channels_half; i++) {
                decoded_data[i] = (data[i] - 0x7F) * kInt8_inv;
                decoded_data[samples_with_channels - i] = (data[samples_with_channels - i] - 0x7F) * kInt8_inv;
            }
        } break;
        case AV_SAMPLE_FMT_S16: {
            int16_t *data = (int16_t*)*frame -> data;
            for (int i = 0; i < samples_with_channels_half; i++) {
                decoded_data[i] = data[i] * kInt16_inv;
                decoded_data[samples_with_channels - i] = data[samples_with_channels - i] * kInt16_inv;
            }
        }
            break;
        case AV_SAMPLE_FMT_S32:
        {
            int32_t *data = (int32_t*)*frame -> data;
            for (int i = 0; i < samples_with_channels_half; i++) {
                decoded_data[i] = data[i] * kInt32_inv;
                decoded_data[samples_with_channels - i] = data[samples_with_channels - i] * kInt32_inv;
            }
        }
            break;
        case AV_SAMPLE_FMT_FLT:
            memcpy(decoded_data, *frame -> data, temp.size());
            break;
        case AV_SAMPLE_FMT_DBL:
        {
            double *data = (double*)*frame -> data;
            for (int i = 0; i < samples_with_channels_half; i++) {
                decoded_data[i] = data[i];
                decoded_data[samples_with_channels - i] = data[samples_with_channels - i];
            }
        }
            break;
        case AV_SAMPLE_FMT_U8P:
        {
            uint8_t **data = (uint8_t**)frame -> extended_data;
            for (int i = 0; i < frame -> nb_samples; ++i) {
                for (int ch = 0; ch < codec_context -> channels; ++ch) {
                    *decoded_data++ = (data[ch][i] - 0x7F) * kInt8_inv;
                }
            }
        }
            break;
        case AV_SAMPLE_FMT_S16P:
        {
            uint16_t **data = (uint16_t**)frame -> extended_data;
            for (int i = 0; i < frame -> nb_samples; ++i) {
                for (int ch = 0; ch < codec_context -> channels; ++ch) {
                    *decoded_data++ = data[ch][i] * kInt16_inv;
                }
            }
        }
            break;
        case AV_SAMPLE_FMT_S32P:
        {
            uint32_t **data = (uint32_t**)frame -> extended_data;
            for (int i = 0; i < frame -> nb_samples; ++i) {
                for (int ch = 0; ch < codec_context -> channels; ++ch) {
                    *decoded_data++ = data[ch][i] * kInt32_inv;
                }
            }
        }
            break;
        case AV_SAMPLE_FMT_FLTP:
        {
            float **data = (float**)frame -> extended_data;
            for (int i = 0; i < frame -> nb_samples; ++i) {
                for (int ch = 0; ch < codec_context -> channels; ++ch) {
                    *decoded_data++ = data[ch][i];
                }
            }
        }
            break;
        case AV_SAMPLE_FMT_DBLP:
        {
            double **data = (double**)frame -> extended_data;
            for (int i = 0; i < frame -> nb_samples; ++i) {
                for (int ch = 0; ch < codec_context -> channels; ++ch) {
                    *decoded_data++ = data[ch][i];
                }
            }
        }
            break;
        default: {
            qWarning("Unsupported audio format: %d", codec_context -> sample_fmt);
            return;
        }
    }

    outputStream -> addBuffer(temp);
}

void AudioStream::fillFormat(QAudioFormat & format) {
//    format.setSampleRate(codec_context -> sample_rate);
//    format.setChannelCount(2);
//    format.setSampleSize(16);
//    format.setCodec("audio/pcm");
//    format.setByteOrder(QAudioFormat::LittleEndian);
//    format.setSampleType(QAudioFormat::SignedInt);




    format.setCodec("audio/pcm");
    format.setSampleRate(codec_context -> sample_rate);//selectSampleRate(codec));
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(codec_context -> channels);

    AVSampleFormat compatibleCodec = codec_context -> sample_fmt; //compatibleCodecType(codec);

    if (compatibleCodec == AV_SAMPLE_FMT_U8) { ///< unsigned 8 bits
        format.setSampleSize(8);
        format.setSampleType(QAudioFormat::UnSignedInt);
    } else if (compatibleCodec == AV_SAMPLE_FMT_S16) { ///< signed 16 bits
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
    } else if (compatibleCodec == AV_SAMPLE_FMT_S32) { ///< signed 32 bits
        format.setSampleSize(32);
        format.setSampleType(QAudioFormat::SignedInt);
    } else if (compatibleCodec == AV_SAMPLE_FMT_FLT) { ///< float
        format.setSampleSize(32);
        format.setSampleType(QAudioFormat::Float);
    } else {
        resampleInit(compatibleCodec);
//        AV_SAMPLE_FMT_DBL,         ///< double
//        AV_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
//        AV_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
//        AV_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
//        AV_SAMPLE_FMT_FLTP,        ///< float, planar
//        AV_SAMPLE_FMT_DBLP,        ///< double, planar
        format.setSampleRate(44100);
        format.setChannelCount(2);
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::UnSignedInt);
    }

    qDebug() << format;

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if (!info.isFormatSupported(format)) {
        qWarning() << "raw audio format not supported by backend, cannot play audio.";
        format = info.nearestFormat(format);
    }

    qDebug() << format;
}

double AudioStream::calcPts(AVPacket * packet) {
    //DO: add resample changes correction to the calculation

    if (packet -> pts != AV_NOPTS_VALUE) {
        //context -> audio_clock = av_q2d(stream -> time_base) * packet -> pts;
        clock = av_q2d(stream -> time_base) * packet -> pts;
    } else {
        int data_size = av_samples_get_buffer_size(
                    NULL,
                    codec_context -> channels,
                    frame -> nb_samples,
                    codec_context -> sample_fmt,
                    1
        );

        /* if no pts, then compute it */
        clock += (double)data_size /
        (
            codec_context -> channels *
            codec_context -> sample_rate *
            av_get_bytes_per_sample(codec_context -> sample_fmt)
        );
    }

    qDebug() << "AUDIO PTS " << av_q2d(stream -> time_base) << " : " << clock;

    return clock;
}

//TODO: require to update settings for fillFormat
void AudioStream::resampleInit(AVSampleFormat sampleFormat) {
    resampleRequire = true;


//    mResampleCtx = ffmpeg::swr_alloc();
//ffmpeg::av_opt_set_int(mResampleCtx, "in_channel_layout", AV_CH_LAYOUT_STEREO, 0);
//ffmpeg::av_opt_set_int(mResampleCtx, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
//ffmpeg::av_opt_set_int(mResampleCtx, "in_sample_rate", 48000, 0);
//ffmpeg::av_opt_set_int(mResampleCtx, "out_sample_rate", 48000, 0);
//ffmpeg::av_opt_set_sample_fmt(mResampleCtx, "in_sample_fmt", ffmpeg::AV_SAMPLE_FMT_FLTP, 0);
//ffmpeg::av_opt_set_sample_fmt(mResampleCtx, "out_sample_fmt", ffmpeg::AV_SAMPLE_FMT_S16, 0);
//if (swr_init(mResampleCtx) < 0) {
//qDebug() << "SWR_INIT ERROR";
//}

//int dst_linesize;
//int dst_nb_samples = 4096;
//ffmpeg::av_samples_alloc(&mResampleBuffer,
//&dst_linesize,
//2,
//dst_nb_samples,
//ffmpeg::AV_SAMPLE_FMT_S16,
//0);


    // AVCodec however decodes audio as float, which we can't use directly
    resampleContext = swr_alloc();
    av_opt_set_int(resampleContext, "in_channel_layout", codec_context -> channel_layout, 0);
    av_opt_set_int(resampleContext, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0); // 2 channels
    av_opt_set_int(resampleContext, "in_sample_rate", codec_context -> sample_rate, 0);
    av_opt_set_int(resampleContext, "out_sample_rate", 44100, 0);
    av_opt_set_sample_fmt(resampleContext, "in_sample_fmt", sampleFormat, 0);
    av_opt_set_sample_fmt(resampleContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    if (swr_init(resampleContext) < 0) {
        qDebug() << "SWR_INIT ERROR";
    }

    int dst_linesize;
    resample_nb_samples = 4096; // default is 1024
    av_samples_alloc(&resampleBuffer,
        &dst_linesize,
        2, // 2 channels
        resample_nb_samples,
        AV_SAMPLE_FMT_S16,
    0);
}

AVSampleFormat AudioStream::compatibleCodecType(AVCodec *codec) {
    if (!codec -> sample_fmts)
        return AV_SAMPLE_FMT_S16;

    const enum AVSampleFormat *sample_fmts;
    AVSampleFormat best_format = AV_SAMPLE_FMT_U8;

    sample_fmts = codec -> sample_fmts;
    while (*sample_fmts) {
        if (*sample_fmts == AV_SAMPLE_FMT_S16 || *sample_fmts == AV_SAMPLE_FMT_S32
                || *sample_fmts == AV_SAMPLE_FMT_FLT || *sample_fmts == AV_SAMPLE_FMT_DBL)
            best_format = *sample_fmts;
        sample_fmts++;
    }

    return best_format;
}

/* just pick the highest supported samplerate */
int AudioStream::selectSampleRate(AVCodec *codec) {
    if (!codec -> supported_samplerates)
        return 44100;

    int best_samplerate = 0;
    const int *p = codec -> supported_samplerates;

    while (*p) {
        best_samplerate = FFMAX(*p, best_samplerate);
        p++;
    }

    return best_samplerate;
}

/* select layout with the highest channel count */
int AudioStream::selectChannelLayout(AVCodec *codec) {
    if (!codec -> channel_layouts)
        return AV_CH_LAYOUT_STEREO;

    const uint64_t *p;
    uint64_t best_ch_layout = 0;
    int best_nb_channels = 0;

    p = codec -> channel_layouts;
    while (*p) {
        int nb_channels = av_get_channel_layout_nb_channels(*p);

        if (nb_channels > best_nb_channels) {
            best_ch_layout = *p;
            best_nb_channels = nb_channels;
        }
        p++;
    }

    return best_ch_layout;
}
