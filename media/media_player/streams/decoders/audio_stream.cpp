#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , isPlanar(false)
    , resampleRequire(false)
    , resampler(0)
    , defaultChannelLayout(AV_CH_LAYOUT_STEREO) {

    if (state) {
        isPlanar = (codec_context -> channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codec_context -> sample_fmt));
        //    std::cout << "The audio stream (and its frames) have too many channels to fit in\n"
        //              << "frame->data. Therefore, to access the audio data, you need to use\n"
        //              << "frame->extended_data to access the audio data. It's planar, so\n"
        //              << "each channel is in a different element. That is:\n"
        //              << "  frame->extended_data[0] has the data for channel 1\n"
        //              << "  frame->extended_data[1] has the data for channel 2\n"
        //              << "  etc.\n";

    //    bufferLimit = 500;
        QAudioFormat format;
        fillFormat(format);

        outputStream = new AudioOutputStream(this, format, priority);
    //    outputStream = new PortAudioOutputStream(this, format, priority);
    }
}

AudioStream::~AudioStream() {
    qDebug() << "Audion stream";
    outputStream -> stop();
    outputStream -> wait();

    delete resampler;
}

void AudioStream::stop() {
    qDebug() << "AUDIO STOP";
    outputStream -> stop();
    MediaStream::stop();
}

void AudioStream::suspendOutput() {
    pauseRequired = true;
    outputStream -> suspend();
}
void AudioStream::resumeOutput() {
    pauseRequired = false;
    outputStream -> resume();
}

void AudioStream::routine() {
    mutex -> lock();
    if (packets.isEmpty()) {
        mutex -> unlock();
        pauseRequired = finishAndPause && outputStream -> isFinished();
        if (finishAndExit && outputStream -> isFinished()) stop();
        msleep(waitMillis);
        return;
    }

    AVPacket * packet = packets.takeFirst();
    mutex -> unlock();

    int len, got_frame;

    while (packet -> size > 0) {
        len = avcodec_decode_audio4(codec_context, frame, &got_frame, packet);

        if (len < 0) {
            qDebug() << "Error while decoding audio frame";
            av_free_packet(packet);
            return;
        }

        if (got_frame) {
//            manualResample();
            QByteArray * ar = new QByteArray();

            if (resampleRequire) {
                if (!resampler -> proceed(frame, ar))
                    qDebug() << "RESAMPLER FAIL";
            } else {
                ar -> append((const char*)frame -> data[0], frame -> linesize[0]);
//                QByteArray ar((const char*)*frame -> extended_data, frame -> nb_samples);
            }

            outputStream -> addBuffer(ar);
            MasterClock::instance() -> setAudio(calcPts(packet));
            msleep(((double)ar -> size()) / bytesPerSecond() * 100);

//            av_frame_unref(frame);
        } else {
            qDebug() << "Could not get audio data from this frame";
        }

        av_frame_unref(frame);
        packet -> size -= len;
        packet -> data += len;
    }

    av_free_packet(packet);
}

//wtf
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

    outputStream -> addBuffer(&temp);
}

void AudioStream::fillFormat(QAudioFormat & format) {
    bool approxSettings = false, notSupport = false;
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    format.setCodec("audio/pcm");
    format.setSampleRate(codec_context -> sample_rate);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(codec_context -> channels);
    format.setSampleSize(av_get_bytes_per_sample(codec_context -> sample_fmt) * 8);

    switch (codec_context -> sample_fmt) {
        case AV_SAMPLE_FMT_U8: { ///< unsigned 8 bits
            format.setSampleType(QAudioFormat::UnSignedInt);
        break;}

        case AV_SAMPLE_FMT_S16: { ///< signed 16 bits
            format.setSampleType(QAudioFormat::SignedInt);
        break;}

        case AV_SAMPLE_FMT_S32: { ///< signed 32 bits
            format.setSampleType(QAudioFormat::SignedInt);
        break;}

        case AV_SAMPLE_FMT_FLT: { ///< float
            format.setSampleType(QAudioFormat::Float);
        break;}

        default:  {
            //        AV_SAMPLE_FMT_DBL,         ///< double
            //        AV_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
            //        AV_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
            //        AV_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
            //        AV_SAMPLE_FMT_FLTP,        ///< float, planar
            //        AV_SAMPLE_FMT_DBLP,        ///< double, planar

            format.setSampleSize(32);
            format.setSampleType(QAudioFormat::Float);
            notSupport = true;
        }
    }

    qDebug() << format;
    if ((approxSettings = !info.isFormatSupported(format))) {
        qWarning() << format << " format not supported by backend, set approx format.";
        format = info.nearestFormat(format);
        qDebug() << format;
    }

    if (approxSettings || notSupport) {
        resampler = new AudioResampler();

        enum AVSampleFormat sampleFormatOut = MediaPlayerUtils::toSampleFormat(format.sampleSize());

        resampleRequire = resampler -> init(
                        codec_context -> sample_fmt,
                        sampleFormatOut,
                        codec_context -> sample_rate,
                        format.sampleRate(),
                        MediaPlayerUtils::checkChannelLayout(codec_context -> channel_layout, codec_context -> channels),
//                        MediaPlayerUtils::checkChannelLayout(MediaPlayerUtils::selectChannelLayout(codec), codec_context -> channels),
                        MediaPlayerUtils::checkChannelLayout(defaultChannelLayout, format.channelCount())
                    );
    }
}

double AudioStream::calcPts(AVPacket * packet) {
    double clock;

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

//    qDebug() << "AUDIO PTS " << av_q2d(stream -> time_base) << " : " << clock;

    return clock;
}

int AudioStream::bytesPerSecond() {
    if (resampleRequire)
        return resampler -> bytesPerSecond();
    else
        return codec_context -> sample_rate * codec_context -> channels * av_get_bytes_per_sample(codec_context -> sample_fmt);
}
