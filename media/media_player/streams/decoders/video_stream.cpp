#include "video_stream.h"
#include <QApplication>

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority) {

    output = new GLOutput();
    resampler = new VideoResampler(codec_context -> pix_fmt);
}

VideoStream::~VideoStream() {
    delete output;
    delete resampler;  
}

void VideoStream::suspendOutput() {

}
void VideoStream::resumeOutput() {

}

void VideoStream::stop() {
    qDebug() << "VIDEO STOP";
    output -> setFrame(new VideoFrame(0, 0));
//    output -> close();
    MediaStream::stop();
}

void VideoStream::routine() {
    mutex -> lock();
    if (packets.isEmpty()) {
        mutex -> unlock();
        pauseRequired = finishAndPause;
        if (finishAndExit) stop();
        msleep(waitMillis);
        return;
    }

    AVPacket * packet = packets.takeFirst();
    mutex -> unlock();

    int len, got_picture;
    int width = codec_context -> width, height = codec_context -> height;
    QImage * img = 0;

    while (packet -> size > 0) {
        len = avcodec_decode_video2(codec_context, frame, &got_picture, packet);

        if (len < 0) {
            qDebug() << "Error while decoding video frame";
            return;
        }

        if (got_picture) {
            img = resampler -> proceed(frame, width, height, width, height);
            MasterClock::instance() -> setVideoNext(calcPts());

//            av_frame_unref(frame);
        } else {
            qDebug() << "Could not get a full picture from this frame";
        }

        packet -> size -= len;
        packet -> data += len;
        av_frame_unref(frame);
    }

    av_free_packet(packet);

    if (img) {
        uint delay = MasterClock::instance() -> computeVideoDelay();
        output -> setFrame(new VideoFrame(img, delay));
        if (delay > 0) msleep(delay);
    }
}

double VideoStream::calcPts() {
    double pts = frame -> pkt_dts;
    if (pts == AV_NOPTS_VALUE) { pts = frame -> pkt_pts; }
    if (pts == AV_NOPTS_VALUE) { pts = 0; }
    pts *= av_q2d(stream -> time_base);
    pts = syncPts(frame, pts);

//    qDebug() << "VIDEO PTS " << av_q2d(stream -> time_base) << " : " << pts;
    return pts;
}

double VideoStream::syncPts(AVFrame *src_frame, double pts) {
    double clock;

    if(pts != 0) {
        /* if we have pts, set video clock to it */
        clock = pts;
    } else {
        /* if we aren't given a pts, set it to the clock */
        pts = MasterClock::instance() -> video();
    }

    /* update the video clock */
    double frame_delay = av_q2d(codec_context -> time_base);
    /* if we are repeating a frame, adjust clock accordingly */
    frame_delay += src_frame -> repeat_pict * (frame_delay * 0.5);
    MasterClock::instance() -> iterateVideo(frame_delay);

    return pts;
}
