#include "video_resampler.h"
#include "qdebug.h"

// only_conversion - did not output result as QImage if possible
VideoResampler::VideoResampler(AVCodecContext * codec_context, bool hardwareConversion, enum AVPixelFormat pixel_format_out) :
      pixelFormatIn(codec_context -> pix_fmt)
    , pixelFormatOut(pixel_format_out)
    , hardware_conversion(hardwareConversion)
    , settings(0)
    , resampleContext(0)
{
    img_format = VideoTypes::toQImageFormat(pixelFormatOut);
    compatible = VideoTypes::isCompatible(codec_context -> pix_fmt); //pixelFormatOut
    RGBFrame = av_frame_alloc();

    settings = new VideoSettings(
                (!hardware_conversion && compatible) ? codec_context -> pix_fmt : pixelFormatOut,
                codec_context -> colorspace,
                codec_context -> width,
                codec_context -> height
    );
}

VideoResampler::~VideoResampler() {
    delete settings;
    av_frame_free(&RGBFrame);
    sws_freeContext(resampleContext);
}

VideoBuffer * VideoResampler::proceed(AVFrame *& frame, int widthOut, int heightOut) {
    if (!hardware_conversion && compatible) {
        AVFrame * old_frame = frame;
        frame = av_frame_alloc();
//        frame -> pkt_dts = old_frame -> pkt_dts;
//        frame -> pkt_pts = old_frame -> pkt_pts;
//        frame -> repeat_pict = old_frame -> repeat_pict;
        av_frame_copy_props(frame, old_frame);

        return new VideoBuffer(old_frame, settings);


//        AVPicture * newPict = new AVPicture();
//        if (avpicture_alloc(newPict, (AVPixelFormat)frame -> format, frame -> width, frame -> height) == 0) {
//            av_picture_copy(newPict, (AVPicture *)frame, (AVPixelFormat)frame -> format, frame -> width, frame -> height);
//        } else {
//            qDebug() << "HUDO";
//            return 0;
//        }
//        return new VideoBuffer(newPict, settings);
    } else return toQImage(frame, widthOut, heightOut);
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
VideoBuffer * VideoResampler::toQImage(AVFrame * frame, int widthOut, int heightOut) {
    if (img_format == QImage::Format_Invalid) return 0;

    QImage * img = new QImage(widthOut, heightOut, img_format);
    RGBFrame -> width = widthOut;
    RGBFrame -> height = heightOut;
        // Assign appropriate parts of buffer to image planes in RGBFrame
    avpicture_fill((AVPicture *)RGBFrame, img -> bits(), pixelFormatOut, widthOut, heightOut);

    resampleContext = sws_getCachedContext(
                resampleContext,
                frame -> width,
                frame -> height,
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
    sws_scale(resampleContext, frame -> data, frame -> linesize, 0, frame -> height, RGBFrame -> data, RGBFrame -> linesize);
    return new VideoBuffer(img, settings);
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
