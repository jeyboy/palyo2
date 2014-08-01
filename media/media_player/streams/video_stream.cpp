#include "video_stream.h"

VideoStream::VideoStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority)
    : MediaStream(context, streamIndex, parent, priority) {

    RGBFrame = avcodec_alloc_frame();
}

void VideoStream::suspendOutput() {

}
void VideoStream::resumeOutput() {

}

bool VideoStream::decodeFrame(unsigned char* bytes, int size) {
    if (size <= 0)
        return false;

//    mPacket.size = size;
//    mPacket.data = bytes;

//    int len, got_picture;
//    bool hasPicture = false;
//    while (mPacket.size > 0) {
//        len = ffmpeg::avcodec_decode_video2(mCodecCtx, mPicture, &got_picture, &mPacket);

//        if (len < 0) {
//            qDebug() << "Error while decoding video frame";
//            return false;
//        }

//        if (got_picture) {
//            if (!mPictureRGB || mPictureRGB->width != mCodecCtx->width || mPictureRGB->height != mCodecCtx->height) {
//                // Determine required buffer size and allocate buffer
//                int numBytes = ffmpeg::avpicture_get_size(ffmpeg::PIX_FMT_RGB24, mCodecCtx->width, mCodecCtx->height);

//                mPictureRGB->width = mCodecCtx->width;
//                mPictureRGB->height = mCodecCtx->height;

//                if (mRGBBuffer)
//                    delete[] mRGBBuffer;

//                mRGBBuffer = new unsigned char[numBytes];

//                // Assign appropriate parts of buffer to image planes in mPictureRGB
//                avpicture_fill((ffmpeg::AVPicture *)mPictureRGB, mRGBBuffer, ffmpeg::PIX_FMT_RGB24, mCodecCtx->width, mCodecCtx->height);
//            }

//            // Convert to QImage
//            int w = mCodecCtx->width;
//            int h = mCodecCtx->height;

//            /*if (w > 1920 || h > 1920)
//                qDebug() << "Unexpected size! " << w << " x " << h;*/

//            if (mLastRendered) {
//                mConvertCtx = ffmpeg::sws_getCachedContext(mConvertCtx, w, h, mCodecCtx->pix_fmt, w, h, ffmpeg::PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

//                if(mConvertCtx == NULL) {
//                    qDebug() << "Cannot initialize the conversion context!";
//                    return false;
//                }

//                // Convert to RGB
//                sws_scale(mConvertCtx, mPicture->data, mPicture->linesize, 0, mCodecCtx->height, mPictureRGB->data, mPictureRGB->linesize);

//                // Convert the frame to QImage
//                if (mLastFrame.width() != w || mLastFrame.height() != h || mLastFrame.format() != QImage::Format::Format_RGB888) {
//                    mLastFrame = QImage(w,h,QImage::Format_RGB888);
//                }

//                for (int y=0; y < h; y++) {
//                    memcpy(mLastFrame.scanLine(y), mPictureRGB->data[0]+y*mPictureRGB->linesize[0], w*3);
//                }
//            }

//            hasPicture = true;
//        } else {
//            qDebug() << "Could not get a full picture from this frame";
//        }

//        mPacket.size -= len;
//        mPacket.data += len;
//    }

//    return hasPicture;
}

void VideoStream::routine() {

}
