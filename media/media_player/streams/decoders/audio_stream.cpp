#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, int streamIndex)
    : IMediaStream(context, streamIndex), QIODevice(parent)
    , resampleRequire(false)
    , resampler(0)
    , defaultChannelLayout(AV_CH_LAYOUT_STEREO) {

    if (valid) {
        open(QIODevice::ReadWrite);


//        isPlanar = (codec_context -> channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codec_context -> sample_fmt));
        //    std::cout << "The audio stream (and its frames) have too many channels to fit in\n"
        //              << "frame->data. Therefore, to access the audio data, you need to use\n"
        //              << "frame->extended_data to access the audio data. It's planar, so\n"
        //              << "each channel is in a different element. That is:\n"
        //              << "  frame->extended_data[0] has the data for channel 1\n"
        //              << "  frame->extended_data[1] has the data for channel 2\n"
        //              << "  etc.\n";

        QAudioFormat format;
        fillFormat(format);

        output = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
        output -> setVolume(1.0);
        output -> start(this);

//        outputStream = new AudioOutputStream(this, bytesPerSecond(), format, priority);
    //    outputStream = new PortAudioOutputStream(this, format, priority);
    }
}

AudioStream::~AudioStream() {
    qDebug() << "Audion stream";
    output -> stop();

    delete resampler;
}

//void AudioStream::stop() {
//    qDebug() << "AUDIO STOP";
//    output -> stop();
//    MediaStream::stop();
//}

void AudioStream::suspendOutput() {
    pause = true;
    output -> suspend();
}
void AudioStream::resumeOutput() {
    pause = false;
    output -> resume();
}

//TODO: add check on maxlen overflow
//TODO: check situation when one packet contain more than one frame
qint64 AudioStream::readData(char *data, qint64 maxlen) {
    if (maxlen == 0) return 0;

//    if (pause) {
////        memset(data, 0, maxlen);
//        return 0;
//    }

    int reslen = 0;
    int len, got_frame;
    AVPacket * packet;

    while(true) {
        if (packets.isEmpty()) {
            qDebug() << "IS EMPTY";
            memset(data, 0, maxlen / 2);
            return maxlen / 2;
//            return reslen;
        }

        mutex -> lock();
        packet = packets.takeFirst();
        mutex -> unlock();

        while (packet -> size > 0) {
            len = avcodec_decode_audio4(codec_context, frame, &got_frame, packet);

            if (len < 0) {
                qDebug() << "Error while decoding audio frame";
                av_free_packet(packet);
                return reslen;
            }

            if (got_frame) {
                if (resampleRequire) {
                    if (!resampler -> proceed(frame, data, reslen))
                        qDebug() << "RESAMPLER FAIL";
                } else {
                    memcpy(data, (const char*)frame -> data[0], (reslen = frame -> linesize[0]));
                }

                MasterClock::instance() -> setAudio(calcPts(packet));
            } else {
                qDebug() << "Could not get audio data from this frame";
            }

            packet -> size -= len;
            packet -> data += len;
        }

        av_frame_unref(frame);
        av_freep(frame);

        av_free_packet(packet);
        if (reslen > 0) break;
    }

    return reslen;
//            return -1; // return -1 if no data available anymore
}

qint64 AudioStream::writeData(const char *data, qint64 len) {
    return 0;
}

void AudioStream::sync(double delay, char *data, int & len, qint64 maxlen) {
    int n = output -> format().channelCount();
    int wanted_size = len + output -> format().bytesForDuration(delay * 1000);//(delay * output -> format().sampleRate() * n);
    qDebug() << "DELAY " << delay << " " << len << " NEW " << wanted_size;
    if (wanted_size > maxlen)
        wanted_size = maxlen;

    if(wanted_size < len) {
        /* remove samples */
        len = wanted_size;
    } else if(wanted_size > len) {
        int copyNum = wanted_size - len;
        memcpy((uint8_t *)data + len, (uint8_t *)data + len - copyNum , copyNum);


//        uint8_t *samples_end, *q;
//        int nb;

//        /* add samples by copying final samples */
//        nb = (len - wanted_size);
//        samples_end = (uint8_t *)data + len;
//        q = samples_end + n;
//        while(nb > 0) {
//            memcpy(q, samples_end, n);
//            q += n;
//            nb -= n;
//        }
        len = wanted_size;
    }
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
        clock += (double)data_size / (
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
