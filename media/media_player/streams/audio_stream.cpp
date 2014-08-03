#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , resampleRequire(false) {

    bufferLimit = 500;
    QAudioFormat format;
    fillFormat(format);

    outputStream = new AudioOutputStream(this, format, priority);
}

AudioStream::~AudioStream() {
    qDebug() << "Audion stream";
    outputStream -> stop();
    outputStream -> wait();
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

void AudioStream::routine() {
    mutex -> lock();
    if (packets.isEmpty()) {
        mutex -> unlock();
        return;
    }

    AVPacket * packet = packets.takeFirst();
    mutex -> unlock();

    int samples_output;
    int len, out_size;

    while (packet -> size > 0) {
        out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
        len = avcodec_decode_audio4(codec_context, frame, &out_size, packet);

        if (len < 0) {
            qDebug() << "Error while decoding audio frame";
            return;
        }

        if (out_size > 0) {
            // Resample to S16
            if (resampleRequire) {
                qDebug() << "USE RESAMPLE";
                samples_output = swr_convert(resampleContext, &resampleBuffer, 4096, (const uint8_t**)frame -> extended_data, frame -> nb_samples);

                if (samples_output > 0) {
                    // A frame has been decoded. Queue it to our buffer.

                    QByteArray ar((const char*)resampleBuffer, samples_output * 4);
                    outputStream -> addBuffer(ar);
                }
            } else {
                QByteArray ar((const char*)*frame -> extended_data, frame -> nb_samples);
                outputStream -> addBuffer(ar);
            }
        } else {
            qDebug() << "Could not get audio data from this frame";
        }

        packet -> size -= len;
        packet -> data += len;
    }

    av_free_packet(packet);
}

void AudioStream::fillFormat(QAudioFormat & format) {
//    format.setSampleRate(codec_context -> sample_rate);
//    format.setChannelCount(2);
//    format.setSampleSize(16);
//    format.setCodec("audio/pcm");
//    format.setByteOrder(QAudioFormat::LittleEndian);
//    format.setSampleType(QAudioFormat::SignedInt);




    format.setCodec("audio/pcm");
    format.setSampleRate(selectSampleRate(codec));
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(codec_context -> channels);

    AVSampleFormat compatibleCodec = compatibleCodecType(codec);

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
        qDebug() << "RESAMPLE";
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
    int dst_nb_samples = 4096; // default is 1024
    av_samples_alloc(&resampleBuffer,
        &dst_linesize,
        2, // 2 channels
        dst_nb_samples,
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
