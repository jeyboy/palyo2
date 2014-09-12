#include "video_resampler.h"

VideoResampler::VideoResampler(enum AVPixelFormat pixel_format_in, enum AVPixelFormat pixel_format_out) {
    pixelFormatIn = pixel_format_in;
    pixelFormatOut = pixel_format_out;

    RGBFrame = 0;
    resampleContext = 0;
    RGBFrame = av_frame_alloc();
}

VideoResampler::~VideoResampler() {
    av_frame_free(&RGBFrame);
    delete [] RGBBuffer;
    sws_freeContext(resampleContext);
}

QImage * VideoResampler::proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut) {
    if (!RGBFrame || RGBFrame -> width != widthOut
            || RGBFrame -> height != heightOut) {

        RGBFrame -> width = widthOut;
        RGBFrame -> height = heightOut;

        // Determine required buffer size and allocate buffer
        int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, widthOut, heightOut);

        if (RGBBuffer)
            delete[] RGBBuffer;

        RGBBuffer = new unsigned char[numBytes];

        // Assign appropriate parts of buffer to image planes in RGBFrame
        avpicture_fill((AVPicture *)RGBFrame, RGBBuffer, pixelFormatOut, widthOut, heightOut);
    }

    resampleContext = sws_getCachedContext(
                resampleContext,
                widthIn,
                heightIn,
                pixelFormatIn,
                widthOut,
                heightOut,
                pixelFormatOut,
                SWS_BICUBIC,
                NULL,
                NULL,
                NULL
    );

    if(resampleContext == NULL) {
        qDebug() << "Cannot initialize the conversion context!";
        return 0;
    }

//    * @param table the yuv2rgb coefficients describing the output yuv space, normally ff_yuv2rgb_coeffs[x]
//    * @param inv_table the yuv2rgb coefficients describing the input yuv space, normally ff_yuv2rgb_coeffs[x]
//    * @param brightness 16.16 fixed point brightness correction
//    * @param contrast 16.16 fixed point contrast correction
//    * @param saturation 16.16 fixed point saturation correction
//    * @return -1 if not supported
//    */
//   int sws_setColorspaceDetails(struct SwsContext *c, const int inv_table[4],
//                                int srcRange, const int table[4], int dstRange,
//                                int brightness, int contrast, int saturation);

//    sws_setColorspaceDetails(c, ff_yuv2rgb_coeffs[SWS_CS_DEFAULT],
//        c->srcRange, ff_yuv2rgb_coeffs[SWS_CS_DEFAULT] /* FIXME*/,
//        c->dstRange, 0, 1<<16, 1<<16);

    // Convert to RGB
    sws_scale(resampleContext, frame -> data, frame -> linesize, 0, heightIn, RGBFrame -> data, RGBFrame -> linesize);

    // Convert the frame to QImage
    QImage * img = new QImage(widthOut, heightOut, QImage::Format_RGB888);
    uint8_t * offset = RGBFrame -> data[0];
    int len = widthOut * 3;

    for (int y = 0; y < heightOut; y++, offset += RGBFrame -> linesize[0]) {
        memcpy(img -> scanLine(y), offset, len);
    }

    return img;
}







//            if (!RGBFrame || RGBFrame -> width != width
//                    || RGBFrame -> height != height) {
//                // Determine required buffer size and allocate buffer
//                int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, width, height);

//                RGBFrame -> width = width;
//                RGBFrame -> height = height;

//                if (RGBBuffer)
//                    delete[] RGBBuffer;

//                RGBBuffer = new unsigned char[numBytes];

//                // Assign appropriate parts of buffer to image planes in RGBFrame
//                avpicture_fill((AVPicture *)RGBFrame, RGBBuffer, AV_PIX_FMT_RGB24, width, height);
//            }

//            /*if (width > 1920 || height > 1920)
//                qDebug() << "Unexpected size! " << w << " x " << h;*/

////            qDebug() << codec_context -> pix_fmt;


//            resampleContext = sws_getCachedContext(resampleContext, width, height, codec_context -> pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

//            if(resampleContext == NULL) {
//                qDebug() << "Cannot initialize the conversion context!";
//                return;
//            }

//            // Convert to RGB
//            sws_scale(resampleContext, frame -> data, frame -> linesize, 0, height, RGBFrame -> data, RGBFrame -> linesize);

//            // Convert the frame to QImage
//            QImage * img = new QImage(width, height, QImage::Format_RGB888);

////            if (mLastFrame.width() != w || mLastFrame.height() != h || mLastFrame.format() != QImage::Format::Format_RGB888) {
////                mLastFrame = QImage(w,h,QImage::Format_RGB888);
////            }

//            for (int y = 0; y < height; y++) {
//                memcpy(img -> scanLine(y), RGBFrame -> data[0] + y * RGBFrame -> linesize[0], width * 3);
//            }
