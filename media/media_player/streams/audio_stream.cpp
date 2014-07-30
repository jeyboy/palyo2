#include "audio_stream.h"

AudioStream::AudioStream(AVFormatContext * context, uint streamIndex, Priority priority)
    : Stream(context, streamIndex, priority) {

}

void AudioStream::fillFormat(QAudioFormat & format) {
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
