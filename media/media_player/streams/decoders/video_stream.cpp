#include "video_stream.h"
#include <QApplication>

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , output(0)
    , resampler(0){

    if (valid) {
        output = new VideoOutput(codec_context -> width, codec_context -> height);
        resampler = new VideoResampler(codec_context -> pix_fmt);
    }
}

VideoStream::~VideoStream() {
    delete output;
    delete resampler;  
}

void VideoStream::suspendOutput() {
    pauseRequired = true;
}
void VideoStream::resumeOutput() {
    qDebug() << "RESUME";
    pauseRequired = false;
}

void VideoStream::stop() {
    qDebug() << "VIDEO STOP";
    output -> setFrame(new VideoFrame(0, 0));
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

//    ///////// packet length correction
//    QByteArray encoded;
//    encoded.reserve(packet -> size * 2 + FF_INPUT_BUFFER_PADDING_SIZE);
//    encoded.resize(packet -> size * 2);
//    // also copy  padding data(usually 0)
//    memcpy(encoded.data(), packet -> data, packet -> size);
//    packet -> data = (uint8_t *)encoded.data();
//    ////////

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

            av_frame_unref(frame);

            if (img) {
                int delay = MasterClock::instance() -> computeVideoDelay();
        //        if (delay <= 0) {
                if (delay < 0) { // there is always will be greater or equal to the zero
                    qDebug() << "-----SKIP";
                    delete img;
                } else {
                    qDebug() << "+++++PROC";
                    output -> setFrame(new VideoFrame(img, delay));
                    msleep(delay);
                }
            }
        } else {
            qWarning("Could not get a full picture from this frame");
//            char bla[AV_ERROR_MAX_STRING_SIZE];
//            qWarning("Could not get a full picture from this frame %s", av_make_error_string(bla, AV_ERROR_MAX_STRING_SIZE, len));
//            delete [] bla;
        }

        packet -> size -= len;
        packet -> data += len;
    }

    av_free_packet(packet);
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
    if(pts == 0) {
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
