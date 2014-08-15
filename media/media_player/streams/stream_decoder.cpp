#include "stream_decoder.h"
#include <qDebug>

StreamDecoder::StreamDecoder(AVFormatContext * currContext, QObject * parent) : Stream(parent, QThread::TimeCriticalPriority)
, defaultLang("rus")
, state(true)
, finished(false)
, videoStream(0)
, audioStream(0)
, subtitleStream(0) {

    currFrame = new AVPacket();
    context = currContext;
    findStreams();
    MasterClock::instance() -> setMain((double)av_gettime() / 1000000.0);

    suspendOutput();
    start();
}

StreamDecoder::~StreamDecoder() {
    qDebug() << "decoder";
    videoStream -> stop();
    videoStream -> wait();

    audioStream -> stop();
    audioStream -> wait();

    subtitleStream -> stop();
    subtitleStream -> wait();

//    av_free_packet(currFrame); // maybe not?
    delete currFrame;
}

void StreamDecoder::seek(int64_t target) {
    pauseRequired = true;

//    incr = -60.0;
//    pos = get_master_clock(global_video_state);
//	pos += incr;
//	stream_seek(global_video_state,
//                      (int64_t)(pos * AV_TIME_BASE), incr);

    if (audioStream -> seek(context, target)
            || videoStream -> seek(context, target)
            || subtitleStream -> seek(context, target)) {

        videoStream -> dropPackets();
        audioStream -> dropPackets();
        subtitleStream -> dropPackets();
    }

    pauseRequired = false;
}

void StreamDecoder::suspendOutput() {
    videoStream -> suspendOutput();
    audioStream -> suspendOutput();
    subtitleStream -> suspendOutput();

    videoStream -> suspend();
    audioStream -> suspend();
    subtitleStream -> suspend();
}
void StreamDecoder::resumeOutput() {
    videoStream -> resumeOutput();
    audioStream -> resumeOutput();
    subtitleStream -> resumeOutput();

    videoStream -> resume();
    audioStream -> resume();
    subtitleStream -> resume();
}

void StreamDecoder::routine() {
//    av_init_packet(currFrame);

    if (videoStream -> isBlocked() || audioStream -> isBlocked() || subtitleStream -> isBlocked()) {
        msleep(12);
        return;
    }

    int status = av_read_frame(context, currFrame);

    if (status >= 0) {

//        hasKeyFrame = !!(currFrame.flags & AV_PKT_FLAG_KEY);
//        // what about marking packet as invalid and do not use isCorrupt?
//        isCorrupt = !!(currFrame.flags & AV_PKT_FLAG_CORRUPT);


        //TODO: maybe need preload for each stream type before start to play
        if (currFrame -> stream_index == audioStream -> index() && !audioStream -> isFinished()) {
            audioStream -> decode(currFrame);
            ac++;
        }
        else if (currFrame -> stream_index == videoStream -> index() && !videoStream -> isFinished()) {
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

        if (status == AVERROR_EOF) {
            qDebug() << "DECODER EOF";
            finished = exitRequired = true;
            videoStream -> exitOnComplete();
            audioStream -> exitOnComplete();
            subtitleStream -> exitOnComplete();
        } else {
            qDebug() << "DECODER STOP";
            pauseRequired = true;
            videoStream -> pauseOnComplete();
            audioStream -> pauseOnComplete();
            subtitleStream -> pauseOnComplete();
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
        emit errorOccurred("No one audio or video streams founds");
        state = false;
    }
}
