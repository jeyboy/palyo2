#include "video_resampler.h"
#include "qdebug.h"

// only_conversion - did not output result as QImage if possible
VideoResampler::VideoResampler(enum AVPixelFormat pixel_format_in, enum AVPixelFormat pixel_format_out) :
      pixelFormatIn(pixel_format_in)
    , pixelFormatOut(pixel_format_out)
    , resampleContext(0)
    , img_format(toQImageFormat(pixelFormatOut))
    , compatible(isCompatibleFormat(pixel_format_out))
{
    RGBFrame = av_frame_alloc();
}

VideoResampler::~VideoResampler() {
    av_frame_free(&RGBFrame);
    sws_freeContext(resampleContext);
}

VideoBuffer * VideoResampler::proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut) {
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

    if (compatible) {
        AVPicture * newPict = new AVPicture();
        if (avpicture_alloc(newPict, (AVPixelFormat)frame -> format, frame -> width, frame -> height) == 0) {
            av_picture_copy(newPict, (AVPicture *)frame, (AVPixelFormat)frame -> format, frame -> width, frame -> height);
        } else {
            qDebug() << "HUDO";
            return 0;
        }

//        uint8_t * data[3];

//        int size = frame -> linesize[0] * frame -> height;

//        data[0] = (uint8_t*)av_malloc(size);
//        memcpy(data[0], frame -> data[0], size);

//        size = frame -> linesize[1] * frame -> height / 2;
//        data[1] = (uint8_t*)av_malloc(size);
//        memcpy(data[1], frame -> data[1], size);

//        size = frame -> linesize[2] * frame -> height / 2;
//        data[2] = (uint8_t*)av_malloc(size);
//        memcpy(data[2], frame -> data[2], size);

//        return data;
        return new VideoBuffer(newPict, frame -> width, frame -> height, (AVPixelFormat)frame -> format, frame -> colorspace);
    } else return toQImage(frame, widthIn, heightIn, widthOut, heightOut);
}

////TODO: this method adapted only for AV_PIX_FMT_RGB24
//VideoBuffer<void> * VideoResampler::proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut) {
//    QImage * img = new QImage(widthOut, heightOut, QImage::Format_RGB888);
//    RGBFrame -> width = widthOut;
//    RGBFrame -> height = heightOut;
//        // Assign appropriate parts of buffer to image planes in RGBFrame
//    avpicture_fill((AVPicture *)RGBFrame, img -> bits(), pixelFormatOut, widthOut, heightOut);

//    resampleContext = sws_getCachedContext(
//                resampleContext,
//                widthIn,
//                heightIn,
//                pixelFormatIn,
//                widthOut,
//                heightOut,
//                pixelFormatOut,
//                SWS_FAST_BILINEAR, //SWS_BICUBIC, //SWS_POINT
//                NULL,
//                NULL,
//                NULL
//    );

//    if(resampleContext == NULL) {
//        qDebug() << "Cannot initialize the conversion context!";
//        return 0;
//    }

//    // Convert to RGB
//    sws_scale(resampleContext, frame -> data, frame -> linesize, 0, heightIn, RGBFrame -> data, RGBFrame -> linesize);
//    return new VideoBuffer<QImage>(img, img -> width(), img -> height(), pixelFormatOut);
//}

//TODO: maybe change format linkage on GL formats ?
VideoBuffer * VideoResampler::toQImage(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut) {
    if (img_format == QImage::Format_Invalid) return 0;

    QImage * img = new QImage(widthOut, heightOut, img_format);
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

    // Convert to RGB
    sws_scale(resampleContext, frame -> data, frame -> linesize, 0, heightIn, RGBFrame -> data, RGBFrame -> linesize);
    return new VideoBuffer(img, img -> width(), img -> height(), pixelFormatOut, frame -> colorspace);
}

void VideoResampler::setColorspaceDetails(int brightness, int contrast, int saturation) {
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

    //    const int srcRange = 1;
    //    const int dstRange = 0;
    //    // TODO: SWS_CS_DEFAULT?
    //    sws_setColorspaceDetails(d.sws_ctx, sws_getCoefficients(SWS_CS_DEFAULT)
    //                             , srcRange, sws_getCoefficients(SWS_CS_DEFAULT)
    //                             , dstRange
    //                             , ((d.brightness << 16) + 50)/100
    //                             , (((d.contrast + 100) << 16) + 50)/100
    //                             , (((d.saturation + 100) << 16) + 50)/100
    //                             );
}

QImage::Format VideoResampler::toQImageFormat(PixelFormat fmt) {
    switch(fmt) {
        case AV_PIX_FMT_RGB24:  { return QImage::Format_RGB888; }
        case AV_PIX_FMT_RGB32:  { return QImage::Format_RGB32;  }
        case AV_PIX_FMT_0RGB32: { return QImage::Format_ARGB32; } // TODO: maybe need correct AV_PIX_FMT format
        case AV_PIX_FMT_ARGB:   { return QImage::Format_ARGB32; }
        case AV_PIX_FMT_RGB565: { return QImage::Format_RGB16;  }
        case AV_PIX_FMT_RGB555: { return QImage::Format_RGB555; }
        case AV_PIX_FMT_RGB444: { return QImage::Format_RGB444; }

        default: return QImage::Format_Invalid;
    }
}

bool VideoResampler::isCompatibleFormat(AVPixelFormat fmt) {

    return
            fmt == AV_PIX_FMT_YUV420P ||
            fmt == AV_PIX_FMT_YUV420P ||
            fmt == AV_PIX_FMT_YUYV422 ||
            fmt == AV_PIX_FMT_RGB24 ||
            fmt == AV_PIX_FMT_BGR24 ||
            fmt == AV_PIX_FMT_YUV422P ||
            fmt == AV_PIX_FMT_YUV444P ||
            fmt == AV_PIX_FMT_YUV410P ||
            fmt == AV_PIX_FMT_YUV411P ||
            fmt == AV_PIX_FMT_YUVJ420P ||
            fmt == AV_PIX_FMT_UYVY422 ||
            fmt == AV_PIX_FMT_NV12 ||
            fmt == AV_PIX_FMT_NV21 ||
            fmt == AV_PIX_FMT_ARGB ||
            fmt == AV_PIX_FMT_RGB32 ||
            fmt == AV_PIX_FMT_RGBA ||
            fmt == AV_PIX_FMT_BGRA ||
            fmt == AV_PIX_FMT_RGB565 ||
            fmt == AV_PIX_FMT_RGB555 ||
            fmt == AV_PIX_FMT_BGR565 ||
            fmt == AV_PIX_FMT_BGR555 ||
            fmt == AV_PIX_FMT_YUV420P16LE ||
            fmt == AV_PIX_FMT_YUV420P16BE ||
            fmt == AV_PIX_FMT_YUV422P16LE ||
            fmt == AV_PIX_FMT_YUV422P16BE ||
            fmt == AV_PIX_FMT_YUV444P16LE ||
            fmt == AV_PIX_FMT_YUV444P16BE ||
            fmt == AV_PIX_FMT_YUV420P9BE ||
            fmt == AV_PIX_FMT_YUV420P9LE ||
            fmt == AV_PIX_FMT_YUV420P10BE ||
            fmt == AV_PIX_FMT_YUV420P10LE ||
            fmt == AV_PIX_FMT_YUV422P10BE ||
            fmt == AV_PIX_FMT_YUV422P10LE ||
            fmt == AV_PIX_FMT_YUV444P9BE ||
            fmt == AV_PIX_FMT_YUV444P9LE ||
            fmt == AV_PIX_FMT_YUV444P10BE ||
            fmt == AV_PIX_FMT_YUV444P10LE ||
            fmt == AV_PIX_FMT_YUV422P9BE ||
            fmt == AV_PIX_FMT_YUV422P9LE ||
            fmt == AV_PIX_FMT_YUV420P12BE ||
            fmt == AV_PIX_FMT_YUV420P12LE ||
            fmt == AV_PIX_FMT_YUV420P14BE ||
            fmt == AV_PIX_FMT_YUV420P14LE ||
            fmt == AV_PIX_FMT_YUV422P12BE ||
            fmt == AV_PIX_FMT_YUV422P12LE ||
            fmt == AV_PIX_FMT_YUV422P14BE ||
            fmt == AV_PIX_FMT_YUV422P14LE ||
            fmt == AV_PIX_FMT_YUV444P12BE ||
            fmt == AV_PIX_FMT_YUV444P12LE ||
            fmt == AV_PIX_FMT_YUV444P14BE ||
            fmt == AV_PIX_FMT_YUV444P14LE;
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
