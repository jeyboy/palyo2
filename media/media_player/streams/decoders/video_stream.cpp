#include "video_stream.h"
#include <QApplication>

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    , RGBFrame(0)
    , resampleContext(0)
    , output(0) {

    RGBFrame = av_frame_alloc();
    output = new GLOutput();
}

VideoStream::~VideoStream() {
    delete output;

    av_frame_free(&RGBFrame);
    delete [] RGBBuffer;
    sws_freeContext(resampleContext);
}

void VideoStream::suspendOutput() {

}
void VideoStream::resumeOutput() {

}

double VideoStream::millisPreloaded() {
    return output -> millisPreloaded();
}

void VideoStream::routine() {
    mutex -> lock();
    if (packets.isEmpty()) {
        pauseRequired = finishAndPause;

        mutex -> unlock();
        return;
    }

    AVPacket * packet = packets.takeFirst();
    mutex -> unlock();

    int len, got_picture;
    int width = codec_context -> width, height = codec_context -> height;

    while (packet -> size > 0) {
        len = avcodec_decode_video2(codec_context, frame, &got_picture, packet);

        if (len < 0) {
            qDebug() << "Error while decoding video frame";
            return;
        }

        if (got_picture) {
            if (!RGBFrame || RGBFrame -> width != width
                    || RGBFrame -> height != height) {
                // Determine required buffer size and allocate buffer
                int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, width, height);

                RGBFrame -> width = width;
                RGBFrame -> height = height;

                if (RGBBuffer)
                    delete[] RGBBuffer;

                RGBBuffer = new unsigned char[numBytes];

                // Assign appropriate parts of buffer to image planes in RGBFrame
                avpicture_fill((AVPicture *)RGBFrame, RGBBuffer, AV_PIX_FMT_RGB24, width, height);
            }

            /*if (width > 1920 || height > 1920)
                qDebug() << "Unexpected size! " << w << " x " << h;*/

//            qDebug() << codec_context -> pix_fmt;


            resampleContext = sws_getCachedContext(resampleContext, width, height, codec_context -> pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

            if(resampleContext == NULL) {
                qDebug() << "Cannot initialize the conversion context!";
                return;
            }

            // Convert to RGB
            sws_scale(resampleContext, frame -> data, frame -> linesize, 0, height, RGBFrame -> data, RGBFrame -> linesize);

            // Convert the frame to QImage
            QImage * img = new QImage(width, height, QImage::Format_RGB888);

//            if (mLastFrame.width() != w || mLastFrame.height() != h || mLastFrame.format() != QImage::Format::Format_RGB888) {
//                mLastFrame = QImage(w,h,QImage::Format_RGB888);
//            }

            for (int y = 0; y < height; y++) {
                memcpy(img -> scanLine(y), RGBFrame -> data[0] + y * RGBFrame -> linesize[0], width * 3);
            }

            int ref = clock;
            output -> setFrame(new VideoFrame(img, calcPts() - ref));
            av_frame_unref(frame);
        } else {
            qDebug() << "Could not get a full picture from this frame";
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
    if(pts != 0) {
        /* if we have pts, set video clock to it */
        clock = pts;
    } else {
        /* if we aren't given a pts, set it to the clock */
        pts = clock;
    }

    /* update the video clock */
    double frame_delay = av_q2d(codec_context -> time_base);
    /* if we are repeating a frame, adjust clock accordingly */
    frame_delay += src_frame -> repeat_pict * (frame_delay * 0.5);
    clock += frame_delay;

    return clock;
}
