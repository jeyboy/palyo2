#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , resampleRequire(false)
    , resampler(0)
    , defaultChannelLayout(AV_CH_LAYOUT_STEREO) {

    if (state) {
//        isPlanar = (codec_context -> channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codec_context -> sample_fmt));
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

        outputStream = new AudioOutputStream(this, bytesPerSecond(), format, priority);
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
        pauseRequired = finishAndPause;
        if (finishAndExit) stop();
        msleep(waitMillis);
        return;
    }

    if (!outputStream -> requireNext()) {
        msleep(waitMillis / 2);
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
            QByteArray * ar = new QByteArray();

            if (resampleRequire) {
                if (!resampler -> proceed(frame, ar))
                    qDebug() << "RESAMPLER FAIL";
            } else {
                ar -> append((const char*)frame -> data[0], frame -> linesize[0]);
            }

            outputStream -> addBuffer(ar);
//            msleep((((double)ar -> size()) / bytesPerSecond()) * 1000);
            MasterClock::instance() -> setAudio(calcPts(packet));
        } else {
            qDebug() << "Could not get audio data from this frame";
        }

        av_frame_unref(frame);
        av_freep(frame);

        packet -> size -= len;
        packet -> data += len;
    }

    av_free_packet(packet);
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
