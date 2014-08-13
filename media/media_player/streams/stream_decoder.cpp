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
    MasterClock::instance() -> setMain((double)av_gettime() / 1000000.0);

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

    if (videoStream -> isBlocked() || audioStream -> isBlocked() || subtitleStream -> isBlocked()) {
        msleep(12);
        return;
    }

    int status = av_read_frame(context, currFrame);

    if (status >= 0) {

//        hasKeyFrame = !!(currFrame.flags & AV_PKT_FLAG_KEY);
//        // what about marking packet as invalid and do not use isCorrupt?
//        isCorrupt = !!(currFrame.flags & AV_PKT_FLAG_CORRUPT);


//        ////////////////////////////////// PADDING_DATA RECALC
//            /*!
//              larger than the actual read bytes because some optimized bitstream readers read 32 or 64 bits at once and could read over the end.
//              The end of the input buffer avpkt->data should be set to 0 to ensure that no overreading happens for damaged MPEG streams
//             */
//            QByteArray encoded;
//            encoded.reserve(currFrame -> size + FF_INPUT_BUFFER_PADDING_SIZE);
//            encoded.resize(currFrame -> size);
//            // also copy  padding data(usually 0)
//            memcpy(encoded.data(), currFrame -> data, encoded.capacity());

//            currFrame -> size = encoded.size();
//            currFrame -> data = (uint8_t*)encoded.constData();
//        ////////////////////////////////// PADDING_DATA RECALC


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
