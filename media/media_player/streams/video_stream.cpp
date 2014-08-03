#include "video_stream.h"

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority)
    ,resampleContext(0)
    ,RGBFrame(0) {

    bufferLimit = 50;
    RGBFrame = av_frame_alloc();
}

VideoStream::~VideoStream() {
    av_frame_free(&RGBFrame);
    delete [] RGBBuffer;
    sws_freeContext(resampleContext);
    qDeleteAll(videoBuffer);
}

void VideoStream::suspendOutput() {

}
void VideoStream::resumeOutput() {

}

void VideoStream::routine() {
    mutex -> lock();
    if (packets.isEmpty() || videoBuffer.length() >= bufferLimit) {
        if (packets.isEmpty())
            pauseRequired = finishAndPause;

        mutex -> unlock();
        return;
    }

    qDebug() << "VIDEO PROCEED";
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

            qDebug() << codec_context -> pix_fmt;


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

            videoBuffer.append(img);
        } else {
            qDebug() << "Could not get a full picture from this frame";
        }

        packet -> size -= len;
        packet -> data += len;
    }

    av_free_packet(packet);
}
