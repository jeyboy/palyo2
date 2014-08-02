#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority) {

    //    resampleInit();

    QAudioFormat format;
    fillFormat(format);

    outputStream = new AudioOutputStream(this, format, mutex, priority);
}

AudioStream::~AudioStream() {
    qDebug() << "Audion stream";
    outputStream -> stop();
    outputStream -> wait();
}

bool AudioStream::decodeFrame(unsigned char* bytes, int size) {
    if (size <= 0)
        return false;

//    mPacket.size = size;
//    mPacket.data = bytes;

//    int len, out_size;
//    bool hasOutput = false;

//    while (mPacket.size > 0) {
//        out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
//        len = avcodec_decode_audio4(mCodecCtx, mAudioFrame, &out_size, &mPacket);

//        if (len < 0) {
//            qDebug() << "Error while decoding audio frame";
//            return false;
//        }

//        if (out_size > 0) {
//            // Resample from FLOAT PLANAR to S16
//            int samples_output = swr_convert(mResampleCtx, &mResampleBuffer, 4096, (const uint8_t**)mAudioFrame->extended_data, mAudioFrame->nb_samples);

//            if (samples_output > 0) {
//                // A frame has been decoded. Queue it to our buffer.
//                mAudioMutex.lock();

//                if (mBuffered < AUDIO_BUFFERING) mBuffered++;

//                mAudioBuffer.append((const char*)mResampleBuffer, samples_output*4);
//                mAudioBufferSize.push_back(samples_output*4);

//                mAudioMutex.unlock();
//                hasOutput = true;
//            }
//        } else {
//            qDebug() << "Could not get audio data from this frame";
//        }

//        mPacket.size -= len;
//        mPacket.data += len;
//    }

//    return hasOutput;
}

void AudioStream::stop() {
    outputStream -> stop();
    MediaStream::stop();
}

void AudioStream::suspendOutput() {

}
void AudioStream::resumeOutput() {

}

void AudioStream::routine() {

//    // Dequeue an audio frame
//    if (mAudioBufferSize.size() > 0 && mBuffered >= AUDIO_BUFFERING && mAudioOutput->bytesFree() > 0) {
//        mAudioMutex.lock();

//        int bufferSize = mAudioBufferSize.front();

//        // If we're too slow/accumulating delay, drop audio frames
//        while (mAudioBuffer.size() > MAX_AUDIO_DATA_PENDING) {
//            bufferSize = mAudioBufferSize.front();
//            mAudioBuffer.remove(0, bufferSize);
//            mAudioBufferSize.pop_front();
//        }

//        // Write to our audio channel
//        QByteArray frame = mAudioBuffer.left(bufferSize);
//        mAudioIO->write(frame);
//        mAudioBuffer.remove(0, bufferSize);

//        mAudioMutex.unlock();

//        mAudioBufferSize.pop_front();
//    }

//    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void AudioStream::fillFormat(QAudioFormat & format) {
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleRate(stream -> codec -> sample_rate);
    format.setChannelCount(stream -> codec -> channels);

    AVSampleFormat compatibleCodec = compatibleCodecType(codec);

    if (compatibleCodec == AV_SAMPLE_FMT_U8) { ///< unsigned 8 bits
        format.setSampleSize(8);
        format.setSampleType(QAudioFormat::UnSignedInt);
    } else if (compatibleCodec == AV_SAMPLE_FMT_S16) { ///< signed 16 bits
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::UnSignedInt);
    } else if (compatibleCodec == AV_SAMPLE_FMT_S32) { ///< signed 32 bits
        format.setSampleSize(32);
        format.setSampleType(QAudioFormat::SignedInt);
    } else if (compatibleCodec == AV_SAMPLE_FMT_FLT) { ///< float
        format.setSampleSize(32);
        format.setSampleType(QAudioFormat::Float);
    } else if (compatibleCodec == AV_SAMPLE_FMT_DBL) { ///< double // did not support by QAudioFormat // maybe need conversion to float
        format.setSampleSize(64);
        format.setSampleType(QAudioFormat::Unknown);
    } else {

//        AV_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
//        AV_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
//        AV_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
//        AV_SAMPLE_FMT_FLTP,        ///< float, planar
//        AV_SAMPLE_FMT_DBLP,        ///< double, planar
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
    }

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if (!info.isFormatSupported(format)) {
        qWarning() << "raw audio format not supported by backend, cannot play audio.";
        format = info.nearestFormat(format);
    }
}


//TODO: require to update settings for fillFormat
void AudioStream::resampleInit() {
    // AVCodec however decodes audio as float, which we can't use directly
    resampleContext = swr_alloc();
    av_opt_set_int(resampleContext, "in_channel_layout", AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(resampleContext, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(resampleContext, "in_sample_rate", 48000, 0);
    av_opt_set_int(resampleContext, "out_sample_rate", 48000, 0);
    av_opt_set_sample_fmt(resampleContext, "in_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);
    av_opt_set_sample_fmt(resampleContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    if (swr_init(resampleContext) < 0) {
        qDebug() << "SWR_INIT ERROR";
    }

    int dst_linesize;
    int dst_nb_samples = 4096;
    av_samples_alloc(&mResampleBuffer,
        &dst_linesize,
        2,
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
