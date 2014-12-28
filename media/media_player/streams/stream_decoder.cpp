#include "stream_decoder.h"
#include <qDebug>

StreamDecoder::StreamDecoder(QObject * parent, AVFormatContext * currContext, MasterClock * clock, QSemaphore * sema, MediaAttributes * attrs) : Stream(parent)
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

    setSemaphore(sema);

    findStreams(clock);

    clock -> reset();

    if (isValid()) {
        suspend();
        start();
    }
}

StreamDecoder::~StreamDecoder() {
    semaphore -> release(1);
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
//    qDebug() << "decoder";

//    qDebug() << "WWWWWW " << audioStream -> calcDelay() << " " << videoStream -> calcDelay() << semaphore -> available();
    semaphore -> acquire(1);

    if (pauseRequired)
        return;

    int status;
    bool preload = qMin(videoStream -> calcDelay(), audioStream -> calcDelay()) == 0; //audioStream -> requirePreload() && videoStream -> requirePreload();

//    bool preload = del == 0; //audioStream -> requirePreload() && videoStream -> requirePreload();
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
    //        else if (currFrame -> stream_index == subtitleStream -> index()) {
    //            subtitleStream -> decode(currFrame);
//            }
            else
                av_free_packet(currFrame);

            currFrame = new AVPacket();
        } else {
            qDebug() << "DECODER BLOCK " << " a " << ac << " v " << vc;

            pauseRequired = true;
            emit stateChanged(state = EofDetected);
            emit eofDetectRequired();
            semaphore -> release(1);

            break;
        }

        if (!preload || (preload && (videoStream -> isBlocked() || audioStream -> isBlocked()))) {
//            semaphore -> release(1);
            if (semaphore -> available() > 0)
                semaphore -> acquire(1);
            return;
        }
    }
}

///////////////////////// Private //////////////////////////////////

int StreamDecoder::langStream() {
    return attributes -> streamIndexOfLang(defaultLang);
}

uint StreamDecoder::bestStream(AudioStream * audio, VideoStream * video) {
    if (audio != 0 && audio -> index() > -1)
        return audio -> index();
    return video -> index();
}

void StreamDecoder::findStreams(MasterClock * clock) {
    videoStream = new VideoStream(this, context, clock, semaphore, av_find_best_stream(context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0));
    audioStream = new AudioStream(this, context, clock, semaphore, av_find_best_stream(context, AVMEDIA_TYPE_AUDIO, langStream(), bestStream(audioStream, videoStream), NULL, 0));
    subtitleStream = new SubtitleStream(this, context, clock, semaphore, av_find_best_stream(context, AVMEDIA_TYPE_SUBTITLE, -1, bestStream(audioStream, videoStream), NULL, 0));

    if(!audioStream -> isValid() && !videoStream -> isValid()) {
//        emit errorOccurred("No one audio or video streams founds");
        emit stateChanged(state = NoData);
    } else {
        connect((Stream *)audioStream, SIGNAL(suspended()), (QObject *)clock -> mediaPlayer(), SLOT(threadSuspended()));
        connect((Stream *)videoStream, SIGNAL(suspended()), (QObject *)clock -> mediaPlayer(), SLOT(threadSuspended()));
    }
}
