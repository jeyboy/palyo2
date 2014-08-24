#include "video_stream.h"
#include <QApplication>

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex)
    : IMediaStream(context, streamIndex), QObject(parent)
    , output(0)
    , resampler(0){

    if (valid) {
        output = new VideoOutput(this, codec_context -> width, codec_context -> height);
        resampler = new VideoResampler(codec_context -> pix_fmt);
    }
}

VideoStream::~VideoStream() {
    output -> setFrame(new VideoFrame(0, 0, 0));
    delete output;
    delete resampler;  
}

void VideoStream::suspendOutput() {
    pause = true;
    qDebug() << "SUSPEND";
}
void VideoStream::resumeOutput() {
    pause = false;
    qDebug() << "RESUME";
}

void VideoStream::nextPict() {
    if (pause || packets.isEmpty())
        return;

    int len, got_picture;
    int width = codec_context -> width, height = codec_context -> height;
    QImage * img = 0;
    AVPacket * packet;
    bool exit = false;

    while(true) {
        mutex -> lock();
        packet = packets.takeFirst();
        mutex -> unlock();

    //    ///////// packet length correction
    //    QByteArray encoded;
    //    encoded.reserve(packet -> size * 2 + FF_INPUT_BUFFER_PADDING_SIZE);
    //    encoded.resize(packet -> size * 2);
    //    // also copy  padding data(usually 0)
    //    memcpy(encoded.data(), packet -> data, packet -> size);
    //    packet -> data = (uint8_t *)encoded.data();
    //    ////////

        while (packet -> size > 0) {
            len = avcodec_decode_video2(codec_context, frame, &got_picture, packet);

            if (len < 0) {
                qDebug() << "Error while decoding video frame";
                return;
            }

            if (got_picture) {
                img = resampler -> proceed(frame, width, height, width, height);
    //            MasterClock::instance() -> setVideoNext(calcPts());

                if (img) {
                    exit = true;
                    output -> setFrame(calcPts(new VideoFrame(img, -1, -1)));


    //                int delay = MasterClock::instance() -> computeVideoDelay();
    //        //        if (delay <= 0) {
    //                if (delay < 0) { // there is always will be greater or equal to the zero
    //                    delete img;
    //                } else {
    //                    output -> setFrame(new VideoFrame(img, delay));
    //                    msleep(delay);
    //                }
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

        av_frame_unref(frame);
        av_freep(frame);

        av_free_packet(packet);
        if (exit || packets.isEmpty()) break;
    }
}

VideoFrame * VideoStream::calcPts(VideoFrame * videoFrame) {
    double pts = frame -> pkt_dts;
    if (pts == AV_NOPTS_VALUE) { pts = frame -> pkt_pts; }
    if (pts == AV_NOPTS_VALUE) { pts = 0; }
    pts *= av_q2d(stream -> time_base);
//    if(pts == 0) {
//        /* if we aren't given a pts, set it to the clock */
//        pts = MasterClock::instance() -> video();
//    }
    videoFrame -> pts = pts;
    videoFrame -> next_pts = syncPts(frame);

//    qDebug() << "FRAME " << videoFrame -> pts << " " << videoFrame -> next_pts;
    return videoFrame;
}

double VideoStream::syncPts(AVFrame *src_frame) {
    /* update the video clock */
    double frame_delay = av_q2d(codec_context -> time_base);
    /* if we are repeating a frame, adjust clock accordingly */
    frame_delay += src_frame -> repeat_pict * (frame_delay * 0.5);
    return /*MasterClock::instance() -> video() + */frame_delay;
//    MasterClock::instance() -> iterateVideo(frame_delay);
}
