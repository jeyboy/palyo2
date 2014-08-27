#include "video_stream.h"
#include <QApplication>

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex)
    : MediaStream(context, streamIndex, parent)
    , output(0)
    , resampler(0){

    if (valid) {
        output = new VideoOutput(this, codec_context -> width, codec_context -> height);
        resampler = new VideoResampler(codec_context -> pix_fmt);
        start(QThread::HighestPriority);
    }
}

VideoStream::~VideoStream() {
    output -> setFrame(new VideoFrame(0, 0, 0));
    delete output;
    delete resampler;  

    qDeleteAll(frames);
}

void VideoStream::suspendOutput() {
    pauseRequired = true;
    qDebug() << "SUSPEND";
}
void VideoStream::resumeOutput() {
    pauseRequired = false;
    qDebug() << "RESUME";
}

void VideoStream::dropPackets() {
    IMediaStream::dropPackets();
    qDeleteAll(frames);
    frames.clear();
}

bool VideoStream::isBlocked() {
    return MediaStream::isBlocked() || frames.size() >= FRAMES_LIMIT;
}

void VideoStream::routine() {
    if (pauseRequired || packets.isEmpty())
        return;

    if (frames.size() >= FRAMES_LIMIT) {
        msleep(50);
        return;
    }


    int len, got_picture;
    int width = codec_context -> width, height = codec_context -> height;
    QImage * img = 0;
    AVPacket * packet;

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
            break;
        }

        if (got_picture) {
            img = resampler -> proceed(frame, width, height, width, height);

            if (img)
              frames.append(calcPts(new VideoFrame(img, -1, -1)));
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
}

void VideoStream::nextPict() {
    if (pauseRequired || frames.isEmpty())
        return;

    output -> setFrame(frames.takeFirst());
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
