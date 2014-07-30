#include "stream_decoder.h"
#include <qDebug>

StreamDecoder::StreamDecoder(AVFormatContext * currContext, QObject * parent) : QThread(parent)
, state(true)
, videoStream(0)
, audioStream(0)
, subtitleStream(0) {
    context = currContext;
    findStreams();
}

StreamDecoder::~StreamDecoder() {
    videoStream -> stop();
    videoStream -> wait();

    audioStream -> stop();
    audioStream -> wait();

    subtitleStream -> stop();
    subtitleStream -> wait();

    delete currFrame;
    currFrame = 0;

//    delete videoStream;
//    videoStream = 0;

//    delete audioStream;
//    audioStream = 0;

//    delete subtitleStream;
//    subtitleStream = 0;
}

QAudioFormat StreamDecoder::prepareAudioFormat() {
    QAudioFormat format;
    format.setCodec("audio/pcm");
    audioStream -> fillFormat(format);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if (!info.isFormatSupported(format)) {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        format = info.nearestFormat(format);
    }

    return format;
}

///////////////////////// Private //////////////////////////////////

uint StreamDecoder::bestStream(Stream * audio, Stream * video) {
    if (audio != 0 && audio -> index() > -1)
        return audio -> index();
    return video -> index();
}

void StreamDecoder::findStreams() {
    videoStream = new VideoStream(this, context, av_find_best_stream(context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0));
//    connect(search, SIGNAL(finished()), searchThread, SLOT(deleteLater()));
    audioStream = new AudioStream(this, context, av_find_best_stream(context, AVMEDIA_TYPE_AUDIO, -1, bestStream(audioStream, videoStream), NULL, 0));
    subtitleStream = new SubtitleStream(this, context, av_find_best_stream(context, AVMEDIA_TYPE_SUBTITLE, -1, bestStream(audioStream, videoStream), NULL, 0));

    if(!audioStream -> isValid() && !videoStream -> isValid()) {
        emit errorOccurred("No one audio or video streams founds");
        state = false;
    }
}

bool StreamDecoder::nextFrame() {
    return av_read_frame(context, currFrame) == 0;
}

/* just pick the highest supported samplerate */
int StreamDecoder::selectSampleRate(AVCodec *codec) {
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
int StreamDecoder::selectChannelLayout(AVCodec *codec) {
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
