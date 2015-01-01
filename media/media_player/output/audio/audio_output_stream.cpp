#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority)
    : AudioStream(parent, context, clock, sema, streamIndex, priority), QIODevice(parent) {

    if (valid) {
        QAudioFormat format;
        fillFormat(format);

        output = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, parent);
//        output -> setNotifyInterval(20);
        output -> setVolume(1.0);

        open(QIODevice::ReadOnly);
        output -> start(this);
    }
}

AudioOutputStream::~AudioOutputStream() {
    qDebug() << "Audio output";
    output -> stop();
    output -> reset();
    close();
    delete output;
}

uint AudioOutputStream::getVolume() const { return output -> volume() * 1000; }
void AudioOutputStream::setVolume(uint val) { output -> setVolume((qreal)val / 1000); }
bool AudioOutputStream::deviceInAction() { return output -> state() == QAudio::ActiveState; }

void AudioOutputStream::suspendStream() {
    output -> suspend();
    AudioStream::suspendStream();
}
void AudioOutputStream::resumeStream() {
    AudioStream::resumeStream();
    output -> resume();
}

void AudioOutputStream::fillFormat(QAudioFormat & format) {
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

        // this format is not compatible with ffmpeg
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

qint64 AudioOutputStream::readData(char *data, qint64 maxlen) {
    return fillBuffer(data, maxlen);
}

qint64 AudioOutputStream::writeData(const char */*data*/, qint64 /*len*/) {
    return 0;
}
