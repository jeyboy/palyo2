#include "stream_decoder.h"
#include <qDebug>

StreamDecoder::StreamDecoder(QObject * parent, AVFormatContext * currContext, MasterClock * clock, MediaAttributes * attrs) : Stream(parent)
    , attributes(attrs)
    , ac(0)
    , vc(0)
    , context(currContext)
    , currFrame(new AVPacket())
    , defaultLang("rus")
    , state(Initialization)
    , videoStream(0)
    , audioStream(0)
    , subtitleStream(0) {

    findStreams(clock);

    clock -> reset();

    if (isValid()) {
        suspend();
        start();
    }
}

StreamDecoder::~StreamDecoder() {
    qDebug() << "decoder";

    if (videoStream -> isValid()) {
        videoStream -> stop();
        videoStream -> wait();
    }
    delete videoStream;

    if (audioStream -> isValid()) {
        audioStream -> stop();
        audioStream -> wait();
    }
    delete audioStream;

    if (subtitleStream -> isValid()) {
        subtitleStream -> stop();
        subtitleStream -> wait();
    }
    delete subtitleStream;

    av_free_packet(currFrame);
    delete currFrame;
    qDebug() << "decoder @";
}

void StreamDecoder::seek(int64_t target) {
    skip = true;
    suspend();
    emit stateChanged(state = Seeking);
    emit flushDataRequired();
    avformat_seek_file(context, -1, INT64_MIN, target, INT64_MAX, 0);
    emit eofRejectRequired();
    resume();
    skip = false;
}

void StreamDecoder::suspend() {
    emit stateChanged(state = Suspended);
    Stream::suspend();
    emit suspendRequired();
}
void StreamDecoder::resume() {
    emit stateChanged(state = Resumed);
    Stream::resume();
    emit resumeRequired();
}

QWidget * StreamDecoder::getScreenWidget() const {
    if (videoStream -> isValid())
        return videoStream -> getScreenWidget();
    else return 0;
}

uint StreamDecoder::getVolume() const {
    if (audioStream -> isValid())
        return audioStream -> getVolume();
    else return 0;
}
void StreamDecoder::setVolume(uint val) {
    if (audioStream -> isValid())
        audioStream -> setVolume(val);
}

//TODO: maybe one currFrame init enough
//TODO: while eof Stream::run delay must be minimal
void StreamDecoder::routine() {
//    av_init_packet(currFrame);

    int del = qMin(videoStream -> calcDelay(), audioStream -> calcDelay());
    if (del > 2 || pauseRequired) {
        msleep(del);
        return;
    }

    int status;
    bool preload = del == 0; //audioStream -> requirePreload() && videoStream -> requirePreload();
    qDebug() << "decoder proceed " << preload;
    emit stateChanged(state = Process);

    while (!skip) {
        if (pauseRequired) return;

        status = av_read_frame(context, currFrame);

        if (status >= 0) {

    //        hasKeyFrame = !!(currFrame.flags & AV_PKT_FLAG_KEY);
    //        // what about marking packet as invalid and do not use isCorrupt?

            if (!!(currFrame -> flags & AV_PKT_FLAG_CORRUPT)) {
                qDebug() << "!!!!!!@@@ corrupt";
                av_free_packet(currFrame);
                continue;
            }

            if (audioStream -> isValid() && currFrame -> stream_index == audioStream -> index()) {
                audioStream -> decode(currFrame);
                ac++;
            }
            else if (videoStream -> isValid() && currFrame -> stream_index == videoStream -> index()) {
                videoStream -> decode(currFrame);
                vc++;
            }
    //        else if (currFrame -> stream_index == subtitleStream -> index())
    //            subtitleStream -> decode(currFrame);
            else
                av_free_packet(currFrame);

            currFrame = new AVPacket();
        } else {
            qDebug() << "DECODER BLOCK " << " a " << ac << " v " << vc;

            pauseRequired = true;
            emit stateChanged(state = EofDetected);
            emit eofDetectRequired();

            break;
        }

        if (!preload || (videoStream -> isBlocked() || audioStream -> isBlocked()))
            return;
    }
}

///////////////////////// Private //////////////////////////////////

int StreamDecoder::langStream() {
    int l = attributes -> streamIndexOfLang(defaultLang);
    qDebug() << "HHHHHHHHHHH " << l;
    return l;

//    AVDictionaryEntry * tag = 0;
//    AVDictionary * dict;

//    for(uint i = 0; i < context -> nb_streams; i++) {
//        if (context -> streams[i] -> codec -> codec_type != AVMEDIA_TYPE_AUDIO) continue;
//        dict = context -> streams[i] -> metadata;
//        while ((tag = av_dict_get(dict, "", tag, AV_DICT_IGNORE_SUFFIX))) {
//            if (QString(tag -> key) == "language" && QString(tag -> value) == defaultLang)
//                return i;
//        }
//    }

//    return -1;
}

uint StreamDecoder::bestStream(AudioStream * audio, VideoStream * video) {
    if (audio != 0 && audio -> index() > -1)
        return audio -> index();
    return video -> index();
}

void StreamDecoder::findStreams(MasterClock * clock) {
    videoStream = new VideoStream(this, context, clock, av_find_best_stream(context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0));
    audioStream = new AudioStream(this, context, clock, av_find_best_stream(context, AVMEDIA_TYPE_AUDIO, langStream(), bestStream(audioStream, videoStream), NULL, 0));
    subtitleStream = new SubtitleStream(this, context, clock, av_find_best_stream(context, AVMEDIA_TYPE_SUBTITLE, -1, bestStream(audioStream, videoStream), NULL, 0));

    if(!audioStream -> isValid() && !videoStream -> isValid()) {
//        emit errorOccurred("No one audio or video streams founds");
        emit stateChanged(state = NoData);
    }
}
