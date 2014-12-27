#include "video_stream.h"
#include "misc/screen.h"
#include "media/media_player/output/video/controls/renders/render_types.h"

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority)
    : MediaStream(context, clock, streamIndex, parent, priority)
    , output(0)
    , resampler(0){

    if (valid) {
        setSemaphore(sema);
        calcAspectRatio();

        int width, height;
        Screen::screenSize(width, height);

//        packetsBufferLen = 5;
//        framesBufferLen = 30;

        width = qMin((int)(width * 0.6), codec_context -> width);
        height = qMin((int)(height * 0.6), codec_context -> height);

        RenderType type = gl;

        resampler = new VideoResampler(codec_context, type == hardware || type == gl);
        output = new VideoOutput(this, clock, type == hardware || type == gl ? type : (resampler -> isGLShaderCompatible() ? gl_plus : gl), width, height);
    }
}

VideoStream::~VideoStream() {
    if (valid) {
        flushData();
        output -> setRender(none);
        delete output;
        delete resampler;
    }
}

int VideoStream::calcDelay() {
    return output == 0 || is_attachment ? 50 : frames.size();
}

bool VideoStream::isBlocked() {
    return MediaStream::isBlocked() && (output && frames.size() >= framesBufferLen);
}

void VideoStream::routine() {
    mutex -> lock();
    bool isEmpty = packets.isEmpty();
    mutex -> unlock();

    if (!pauseRequired && isEmpty && eof) suspend();

    if (is_attachment && isEmpty) {
        msleep(100);
        return;
    }

    if (pauseRequired) return;

    if (isEmpty) {
        if (frames.size() <=  framesBufferLen / 2)
            semaphore -> release(semaphore -> available() == 0 ? 1 : 0);
        msleep(2);
        return;
    }



//    if (frames.size() >= framesBufferLen) {
//        msleep(frames.size() / 2);
//        return;
//     } else if (frames.size() <= framesBufferLen / 2)
//        msleep(time_buff * 80);

    int len, got_picture;
    int width = codec_context -> width, height = codec_context -> height;
    VideoBuffer * buff = 0;
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
            qDebug() << "Error while decoding video frame: " << len;
            break;
        }

        if (got_picture) {
            buff = resampler -> proceed(frame, width, height);

            if (buff) {
//                float packet_time = packet -> duration * av_q2d(stream -> time_base);
//                time_buff += packet_time;
//                qDebug() << "vdur " << time_buff;

                frames.append(calcPts(new VideoFrame(clock, buff, -1, -1, aspect_ratio)));
            }
        } else {
            qWarning("Could not get a full picture from this frame: %d", len);
//            char bla[AV_ERROR_MAX_STRING_SIZE];
//            qWarning("Could not get a full picture from this frame %s", av_make_error_string(bla, AV_ERROR_MAX_STRING_SIZE, len));
//            delete [] bla;
        }

        packet -> size -= len;
        packet -> data += len;
    }

//    av_frame_unref(frame);

    av_free_packet(packet);
}

void VideoStream::suspendStream() {
    MediaStream::suspendStream();
}

void VideoStream::resumeStream() {
    clock -> resetMain();
    MediaStream::resume();
}

void VideoStream::changeRenderType(RenderType type) {
    output -> setRender(type);
}

void VideoStream::nextFrame(void *& ret) {
//    mutex -> lock();
    if (pauseRequired)
        ret = new VideoFrame(clock); // create skip frame
    else if (!frames.isEmpty())
        ret = frames.takeFirst();
//    mutex -> unlock();
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
    frame_delay += src_frame -> repeat_pict * frame_delay;// * 0.5);
    return /*MasterClock::instance() -> video() + */frame_delay;
//    MasterClock::instance() -> iterateVideo(frame_delay);
}

void VideoStream::calcAspectRatio() {
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
