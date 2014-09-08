#include "video_stream.h"
#include <QApplication>

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , output(0)
    , resampler(0){

    if (valid) {
        calcAspectRatio();
        output = new VideoOutput(this, codec_context -> width, codec_context -> height);
        resampler = new VideoResampler(codec_context -> pix_fmt);
    }
}

VideoStream::~VideoStream() {
    if (output)
        output -> setFrame(new VideoFrame(0, 0, 0, 0));

    delete output;
    delete resampler;  

    qDeleteAll(frames);
    frames.clear();
}

bool VideoStream::isBlocked() {
    return MediaStream::isBlocked() || frames.size() >= FRAMES_LIMIT;
}

void VideoStream::routine() {
    bool isEmpty = packets.isEmpty();

    if (!pauseRequired && isEmpty && eof) suspend();
    if (pauseRequired) return;

    if (isEmpty || frames.size() >= FRAMES_LIMIT) {
        msleep(2);
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
              frames.append(calcPts(new VideoFrame(img, -1, -1, aspect_ratio)));
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
    if (pauseRequired || frames.isEmpty()) {
//        if (eof) output -> setFrame(new VideoFrame(0,0,0));

//        output -> setPause(eof ? 100 : 0);
        return;
    }

    output -> setFrame(frames.takeFirst());
}

void VideoStream::resumeStream() {
    qDebug() << "!!!!! VIDEO RESUME";
    MasterClock::instance() -> resetMain();
    MediaStream::resume();
}

void VideoStream::flushData() {
    MediaStream::dropPackets();
    qDeleteAll(frames);
    frames.clear();
    avcodec_flush_buffers(codec_context);
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

double VideoStream::calcAspectRatio() {
    aspect_ratio = 0;

    if(codec_context -> sample_aspect_ratio.num != 0) {
        aspect_ratio = av_q2d(codec_context -> sample_aspect_ratio) *
            codec_context -> width / codec_context -> height;
    }

    if(aspect_ratio <= 0) {
        aspect_ratio = (float)codec_context -> width /
            (float)codec_context -> height;
    }
}
