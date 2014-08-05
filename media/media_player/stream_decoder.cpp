#include "stream_decoder.h"
#include <qDebug>

StreamDecoder::StreamDecoder(AVFormatContext * currContext, QObject * parent) : Stream(parent, QThread::TimeCriticalPriority)
, state(true)
, finished(false)
, videoStream(0)
, audioStream(0)
, subtitleStream(0) {

    currFrame = new AVPacket();
    context = currContext;
    findStreams();

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

    int status = av_read_frame(context, currFrame);

    if (status >= 0) {
        if (currFrame -> stream_index == audioStream -> index()) {
            audioStream -> decode(currFrame);
            ac++;
        }
        else if (currFrame -> stream_index == videoStream -> index()) {
            videoStream -> decode(currFrame);
            vc++;
        }
//        else if (currFrame -> stream_index == subtitleStream -> index())
//            subtitleStream -> decode(currFrame);
        else
            av_free_packet(currFrame);

        currFrame = new AVPacket();
    } else {
//        if (ret == AVERROR_EOF)
        qDebug() << "DECODER BLOCK " << " a " << ac << " v " << vc;
        finished = pauseRequired = true;
        videoStream -> pauseOnComplete();
        audioStream -> pauseOnComplete();
        subtitleStream -> pauseOnComplete();
    }
}

///////////////////////// Private //////////////////////////////////

uint StreamDecoder::bestStream(AudioStream * audio, VideoStream * video) {
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
