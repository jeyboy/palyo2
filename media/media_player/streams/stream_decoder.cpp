#include "stream_decoder.h"
#include <qDebug>

StreamDecoder::StreamDecoder(QObject * parent, AVFormatContext * currContext) : Stream(parent)
    , defaultLang("rus")
    , state(true)
    , videoStream(0)
    , audioStream(0)
    , subtitleStream(0) {

    currFrame = new AVPacket();
    context = currContext;
    findStreams();

    MasterClock::instance() -> reset();

    suspend();
    start();
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
}

double StreamDecoder::position() {
    return MasterClock::instance() -> audio();

//    if (audioStream -> isValid())
//        return MasterClock::instance() -> audio(); // orient on the audio stream position
//    else
//        return MasterClock::instance() -> video();
}

void StreamDecoder::seek(int64_t target) {
    suspend();
    emit flushDataRequired();
    avformat_seek_file(context, -1, INT64_MIN, target, INT64_MAX, 0);
    emit eofRejectRequired();
    resume();
}

void StreamDecoder::suspend() {
    Stream::suspend();
    emit suspendRequired();
}
void StreamDecoder::resume() {
    qDebug() << "RESUME";
    Stream::resume();
    emit resumeRequired();
}

//TODO: maybe one currFrame init enough
void StreamDecoder::routine() {
//    av_init_packet(currFrame);
    if ((videoStream -> isBlocked() && audioStream -> isBlocked()) || pauseRequired) {
        msleep(10);
        return;
    }

    int status;
    bool preload = audioStream -> requirePreload() && videoStream -> requirePreload();

    while (true) {
        if (pauseRequired) return;
        qDebug() << "!!! ROUTINE STEP";

        status = av_read_frame(context, currFrame);

        if (status >= 0) {

    //        hasKeyFrame = !!(currFrame.flags & AV_PKT_FLAG_KEY);
    //        // what about marking packet as invalid and do not use isCorrupt?
    //        isCorrupt = !!(currFrame.flags & AV_PKT_FLAG_CORRUPT);

            if (currFrame -> stream_index == audioStream -> index() && audioStream -> isValid()) {
                audioStream -> decode(currFrame);
                ac++;
            }
            else if (currFrame -> stream_index == videoStream -> index() && videoStream -> isValid()) {
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
            emit eofDetectRequired();

            break;
        }

        if (!preload || (videoStream -> isBlocked() || audioStream -> isBlocked())) {
            qDebug() << "ROUTINE PRELOAD";
            break;
        }
    }
}

///////////////////////// Private //////////////////////////////////

int StreamDecoder::langStream() {
    AVDictionaryEntry * tag = 0;
    AVDictionary * dict;

    for(uint i = 0; i < context -> nb_streams; i++) {
        if (context -> streams[i] -> codec -> codec_type != AVMEDIA_TYPE_AUDIO) continue;
        qDebug() << "-----------------------------";
        dict = context -> streams[i] -> metadata;
        while ((tag = av_dict_get(dict, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            qDebug() << tag -> key << " " << tag -> value;
            if (QString(tag -> key) == "language" && QString(tag -> value) == defaultLang)
                return i;
//            delete tag;
        }
    }

    return -1;
}

uint StreamDecoder::bestStream(AudioStream * audio, VideoStream * video) {
    if (audio != 0 && audio -> index() > -1)
        return audio -> index();
    return video -> index();
}

void StreamDecoder::findStreams() {
    videoStream = new VideoStream(this, context, av_find_best_stream(context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0));
//    connect(search, SIGNAL(finished()), searchThread, SLOT(deleteLater()));
    audioStream = new AudioStream(this, context, av_find_best_stream(context, AVMEDIA_TYPE_AUDIO, langStream(), bestStream(audioStream, videoStream), NULL, 0));
    subtitleStream = new SubtitleStream(this, context, av_find_best_stream(context, AVMEDIA_TYPE_SUBTITLE, -1, bestStream(audioStream, videoStream), NULL, 0));

    if(!audioStream -> isValid() && !videoStream -> isValid()) {
//        emit errorOccurred("No one audio or video streams founds");
        state = false;
    }
}
