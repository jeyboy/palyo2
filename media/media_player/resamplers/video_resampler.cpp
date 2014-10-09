#include "video_resampler.h"

VideoResampler::VideoResampler(enum AVPixelFormat pixel_format_in, enum AVPixelFormat pixel_format_out) {
    pixelFormatIn = pixel_format_in;
    pixelFormatOut = pixel_format_out;

    resampleContext = 0;
    RGBFrame = av_frame_alloc();
}

VideoResampler::~VideoResampler() {
    av_frame_free(&RGBFrame);
    sws_freeContext(resampleContext);
}

void * VideoResampler::proceed2(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut) {
    if (frame -> format == AV_PIX_FMT_YUV420P) {
        uint8_t * data[3];

        int size = frame -> linesize[0] * frame -> height;
        data[0] = (uint8_t*)av_malloc(size);
        memcpy(data[0], frame -> data[0], size);

        size = frame -> linesize[1] * frame -> height / 2;
        data[1] = (uint8_t*)av_malloc(size);
        memcpy(data[1], frame -> data[1], size);

        size = frame -> linesize[2] * frame -> height / 2;
        data[2] = (uint8_t*)av_malloc(size);
        memcpy(data[2], frame -> data[2], size);

        return data;


//        int size = avpicture_get_size(frame -> format, frame -> width, frame -> height);
//        uint8_t* buffer = (uint8_t*)av_malloc(size);


        // Initialize the AVFrame
//        AVFrame* dupFrame = avcodec_alloc_frame();
//        dupFrame -> width = widthIn;
//        dupFrame -> height = heightIn;
//        dupFrame -> format = AV_PIX_FMT_YUV420P;


//        // Allocate a buffer large enough for all data
//        int size = avpicture_get_size(frame->format, frame->width, frame->height);
//        uint8_t* buffer = (uint8_t*)av_malloc(size);

//        // Initialize frame->linesize and frame->data pointers
//        avpicture_fill((AVPicture*)frame, buffer, frame->format, frame->width, frame->height);

//        // Copy data from the 3 input buffers
//        memcpy(frame -> data[0], inputBufferY, frame -> linesize[0] * frame -> height);
//        memcpy(frame -> data[1], inputBufferU, frame -> linesize[1] * frame -> height / 2);
//        memcpy(frame -> data[2], inputBufferV, frame -> linesize[2] * frame -> height / 2);



//        // Initialize frame->linesize
//        avpicture_fill((AVPicture*)dupFrame, NULL, frame -> format, frame -> width, frame -> height);

//        // Set frame->data pointers manually
//        dupFrame -> data[0] = inputBufferY;
//        dupFrame -> data[1] = inputBufferU;
//        dupFrame -> data[2] = inputBufferV;

        // Or if your Y, U, V buffers are contiguous and have the correct size, simply use:
        // avpicture_fill((AVPicture*)frame, inputBufferYUV, frame->format, frame->width, frame->height);
    } else return proceed(frame, widthIn, heightIn, widthOut, heightOut);
}

void * VideoResampler::proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut) {
    QImage * img = new QImage(widthOut, heightOut, QImage::Format_RGB888);
    RGBFrame -> width = widthOut;
    RGBFrame -> height = heightOut;
        // Assign appropriate parts of buffer to image planes in RGBFrame
    avpicture_fill((AVPicture *)RGBFrame, img -> bits(), pixelFormatOut, widthOut, heightOut);

    resampleContext = sws_getCachedContext(
                resampleContext,
                widthIn,
                heightIn,
                pixelFormatIn,
                widthOut,
                heightOut,
                pixelFormatOut,
                SWS_FAST_BILINEAR, //SWS_BICUBIC, //SWS_POINT
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
