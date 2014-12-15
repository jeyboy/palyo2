#ifndef VIDEO_SETTINGS_H
#define VIDEO_SETTINGS_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/video_types.h"

struct VideoSettings {
    VideoSettings(AVPixelFormat format, AVColorSpace color_space, int width, int height) :
        bpps(4),
//        bpps_pad(4),
        planes(0)
    {
        colorspace = color_space;
        this -> width = width;
        this -> height = height;
        descriptor = av_pix_fmt_desc_get((fmt = format));
        init();
    }

//    AVCOL_SPC_RGB         = 0,
//    AVCOL_SPC_BT709       = 1,  ///< also ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
//    AVCOL_SPC_UNSPECIFIED = 2,
//    AVCOL_SPC_RESERVED    = 3,
//    AVCOL_SPC_FCC         = 4,
//    AVCOL_SPC_BT470BG     = 5,  ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601
//    AVCOL_SPC_SMPTE170M   = 6,  ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC / functionally identical to above
//    AVCOL_SPC_SMPTE240M   = 7,
//    AVCOL_SPC_YCOCG       = 8,  ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
//    AVCOL_SPC_BT2020_NCL  = 9,  ///< ITU-R BT2020 non-constant luminance system
//    AVCOL_SPC_BT2020_CL   = 10, ///< ITU-R BT2020 constant luminance system
//    AVCOL_SPC_NB,               ///< Not part of ABI

    bool isBT709Colorspace()    { return colorspace == AVCOL_SPC_BT709; }


    bool isRGB()            { return (descriptor -> flags & AV_PIX_FMT_FLAG_RGB) == AV_PIX_FMT_FLAG_RGB; }
    bool isHWACCEL()        { return (descriptor -> flags & AV_PIX_FMT_FLAG_HWACCEL) == AV_PIX_FMT_FLAG_HWACCEL; }
    bool isPlanar()         { return (descriptor -> flags & AV_PIX_FMT_FLAG_PLANAR) == AV_PIX_FMT_FLAG_PLANAR; }
    bool isBigEndian()      { return (descriptor -> flags & AV_PIX_FMT_FLAG_BE) == AV_PIX_FMT_FLAG_BE; }
    bool isBitstream()      { return (descriptor -> flags & AV_PIX_FMT_FLAG_BITSTREAM) == AV_PIX_FMT_FLAG_BITSTREAM; }
    bool hasPalette()       { return (descriptor -> flags & AV_PIX_FMT_FLAG_PAL) == AV_PIX_FMT_FLAG_PAL; }
    bool hasPseudoPalette() { return (descriptor -> flags & AV_PIX_FMT_FLAG_PSEUDOPAL) == AV_PIX_FMT_FLAG_PSEUDOPAL; }
    bool hasAlpha()         { return (descriptor -> flags & AV_PIX_FMT_FLAG_ALPHA) == AV_PIX_FMT_FLAG_ALPHA; }

    int channelsCount() const {
        if (!descriptor) return 0;
        return descriptor -> nb_components;
    }

    VideoTypes::VideoType toVideoType() { return VideoTypes::toVideoType(fmt); }

    //        AVPixelFormat f = av_pix_fmt_swap_endianness(fmt); // need to check this later

    //        return
    //                f == AV_PIX_FMT_RGB24 || f == AV_PIX_FMT_BGR24
    //             || f == AV_PIX_FMT_RGB565 || f == AV_PIX_FMT_BGR565
    //             || f == AV_PIX_FMT_RGB555 || f == AV_PIX_FMT_BGR555
    //             || f == AV_PIX_FMT_ARGB || f == AV_PIX_FMT_RGBA
    //             || f == AV_PIX_FMT_ABGR || f == AV_PIX_FMT_BGRA;
    //    }

    //    bool isPlanar() {
    //        return
    //                fmt == AV_PIX_FMT_YUV420P ||
    //                fmt == AV_PIX_FMT_YUYV422 ||
    //                fmt == AV_PIX_FMT_YUV422P ||
    //                fmt == AV_PIX_FMT_YUV444P ||
    //                fmt == AV_PIX_FMT_YUV410P ||
    //                fmt == AV_PIX_FMT_YUV411P ||
    //                fmt == AV_PIX_FMT_YUVJ420P ||
    //                fmt == AV_PIX_FMT_UYVY422 ||
    //                fmt == AV_PIX_FMT_NV12 ||
    //                fmt == AV_PIX_FMT_NV21 ||
    //                fmt == AV_PIX_FMT_YUV420P16LE ||
    //                fmt == AV_PIX_FMT_YUV420P16BE ||
    //                fmt == AV_PIX_FMT_YUV422P16LE ||
    //                fmt == AV_PIX_FMT_YUV422P16BE ||
    //                fmt == AV_PIX_FMT_YUV444P16LE ||
    //                fmt == AV_PIX_FMT_YUV444P16BE ||
    //                fmt == AV_PIX_FMT_YUV420P9BE ||
    //                fmt == AV_PIX_FMT_YUV420P9LE ||
    //                fmt == AV_PIX_FMT_YUV420P10BE ||
    //                fmt == AV_PIX_FMT_YUV420P10LE ||
    //                fmt == AV_PIX_FMT_YUV422P10BE ||
    //                fmt == AV_PIX_FMT_YUV422P10LE ||
    //                fmt == AV_PIX_FMT_YUV444P9BE ||
    //                fmt == AV_PIX_FMT_YUV444P9LE ||
    //                fmt == AV_PIX_FMT_YUV444P10BE ||
    //                fmt == AV_PIX_FMT_YUV444P10LE ||
    //                fmt == AV_PIX_FMT_YUV422P9BE ||
    //                fmt == AV_PIX_FMT_YUV422P9LE ||
    //                fmt == AV_PIX_FMT_YUV420P12BE ||
    //                fmt == AV_PIX_FMT_YUV420P12LE ||
    //                fmt == AV_PIX_FMT_YUV420P14BE ||
    //                fmt == AV_PIX_FMT_YUV420P14LE ||
    //                fmt == AV_PIX_FMT_YUV422P12BE ||
    //                fmt == AV_PIX_FMT_YUV422P12LE ||
    //                fmt == AV_PIX_FMT_YUV422P14BE ||
    //                fmt == AV_PIX_FMT_YUV422P14LE ||
    //                fmt == AV_PIX_FMT_YUV444P12BE ||
    //                fmt == AV_PIX_FMT_YUV444P12LE ||
    //                fmt == AV_PIX_FMT_YUV444P14BE ||
    //                fmt == AV_PIX_FMT_YUV444P14LE;
    //    }

    //    bool isBigEndian() {
    //        return
    //                fmt == AV_PIX_FMT_YUV420P16BE ||
    //                fmt == AV_PIX_FMT_YUV422P16BE ||
    //                fmt == AV_PIX_FMT_YUV444P16BE ||
    //                fmt == AV_PIX_FMT_YUV420P9BE ||
    //                fmt == AV_PIX_FMT_YUV420P10BE ||
    //                fmt == AV_PIX_FMT_YUV422P10BE ||
    //                fmt == AV_PIX_FMT_YUV444P9BE ||
    //                fmt == AV_PIX_FMT_YUV444P10BE ||
    //                fmt == AV_PIX_FMT_YUV422P9BE ||
    //                fmt == AV_PIX_FMT_YUV420P12BE ||
    //                fmt == AV_PIX_FMT_YUV420P14BE ||
    //                fmt == AV_PIX_FMT_YUV422P12BE ||
    //                fmt == AV_PIX_FMT_YUV422P14BE ||
    //                fmt == AV_PIX_FMT_YUV444P12BE ||
    //                fmt == AV_PIX_FMT_YUV444P14BE;
    //    }

    //    bool isLittleEndian() {
    //        return
    //                fmt == AV_PIX_FMT_YUV420P16LE ||
    //                fmt == AV_PIX_FMT_YUV422P16LE ||
    //                fmt == AV_PIX_FMT_YUV444P16LE ||
    //                fmt == AV_PIX_FMT_YUV420P9LE ||
    //                fmt == AV_PIX_FMT_YUV420P10LE ||
    //                fmt == AV_PIX_FMT_YUV422P10LE ||
    //                fmt == AV_PIX_FMT_YUV444P9LE ||
    //                fmt == AV_PIX_FMT_YUV444P10LE ||
    //                fmt == AV_PIX_FMT_YUV422P9LE ||
    //                fmt == AV_PIX_FMT_YUV420P12LE ||
    //                fmt == AV_PIX_FMT_YUV420P14LE ||
    //                fmt == AV_PIX_FMT_YUV422P12LE ||
    //                fmt == AV_PIX_FMT_YUV422P14LE ||
    //                fmt == AV_PIX_FMT_YUV444P12LE ||
    //                fmt == AV_PIX_FMT_YUV444P14LE;
    //    }

    void init() {
        if (!descriptor) {
            qDebug() << "Descriptor caput";
            return;
        }

        planes = qMax(av_pix_fmt_count_planes(fmt), 0);
        initBpp();
    }
    QString name() const {
        return av_get_pix_fmt_name(fmt);
    }


    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////

    int bytesPerLine(int width, int plane) const {
        return av_image_get_linesize(fmt, width, plane);
    }

    int planeCount() const { return planes; }

    int bitsPerPixel() const { return bpp; }

//    int bitsPerPixelPadded() const {
//        return bpp_pad;
//    }

    int bitsPerPixelPadded(int plane) const {
        if (plane >= bpps_pad.size())
            return 0;
        return bpps_pad[plane];
    }

    int bitsPerPixel(int plane) const {
        if (plane >= bpps.size())
            return 0;
        return bpps[plane];
    }

    int bytesPerPixel() const {
        return (bitsPerPixel() + 7) >> 3;
    }

    int bytesPerPixel(int plane) const {
        return (bitsPerPixel(plane) + 7) >> 3;
    }

//    int chromaWidth(int lumaWidth) const {
//        return -((-lumaWidth) >> descriptor -> log2_chroma_w);
//    }

    int chromaHeight(int lumaHeight) const {
        return -((-lumaHeight) >> descriptor -> log2_chroma_h);
    }

    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////

    void initBpp() {
        bpps.resize(planes);
        bpps_pad.resize(planes);

        bpp = 0;

        int log2_pixels = descriptor -> log2_chroma_w + descriptor -> log2_chroma_h;


        const AVComponentDescriptor * comp;
        int s;
        for (int c = 0; c < descriptor -> nb_components; c++) {
            comp = &descriptor -> comp[c];
            s = c == 1 || c == 2 ? 0 : log2_pixels;

            bpps[comp -> plane] = (comp -> depth_minus1 + 1) << s;
            bpps_pad[comp -> plane] = ((comp -> step_minus1 + 1 + comp -> offset_plus1) << s);

            if(!(descriptor -> flags & AV_PIX_FMT_FLAG_BITSTREAM))
                bpps_pad[comp -> plane] *= comp -> plane == 0 ? 8 : 4; // maybe 4 only for fmt 420P ?

            bpp += bpps[comp -> plane];
            bpp_pad += bpps_pad[comp -> plane];

            bpps[comp -> plane] >>= s;
            bpps_pad[comp -> plane] >>= s;
        }
        bpp >>= log2_pixels;
        bpp_pad >>= log2_pixels;

        qDebug() << bpps_pad;
    }

    QVector<int> bpps, bpps_pad;

    int width, height;
    int bpp, bpp_pad, planes;
    AVColorSpace colorspace;
    AVPixelFormat fmt;

    const AVPixFmtDescriptor * descriptor;
};

#endif // VIDEO_SETTINGS_H











//// FF_API_PIX_FMT
//#ifdef PixelFormat
//#undef PixelFormat
//#endif

//#define FF_HAS_YUV12BITS FFMPEG_MODULE_CHECK(LIBAVUTIL, 51, 73, 101)

//namespace QtAV {

//// TODO: default ctor, dtor, copy ctor required by implicit sharing?
//class VideoFormatPrivate : public QSharedData
//{
//public:
//    int flags() const {
//        if (!pixdesc)
//            return 0;
//        return pixdesc->flags;
//    }
//    int bytesPerLine(int width, int plane) const {
//        return av_image_get_linesize(pixfmt_ff, width, plane);
//    }

//    VideoFormat::PixelFormat pixfmt;
//    AVPixelFormat pixfmt_ff;
//    QImage::Format qpixfmt;

//    AVPixFmtDescriptor *pixdesc;
//private:
//    // from libavutil/pixdesc.c
//    void initBpp() {
//        //TODO: call later when bpp need
//        bpp = 0;
//        bpp_pad = 0;
//        int log2_pixels = pixdesc->log2_chroma_w + pixdesc->log2_chroma_h;
//        for (int c = 0; c < pixdesc->nb_components; c++) {
//            const AVComponentDescriptor *comp = &pixdesc->comp[c];
//            int s = c == 1 || c == 2 ? 0 : log2_pixels; //?
//            bpps[comp->plane] = (comp->depth_minus1 + 1) << s;
//            bpps_pad[comp->plane] = (comp->step_minus1 + 1) << s;
//            if(!(pixdesc->flags & AV_PIX_FMT_FLAG_BITSTREAM))
//                bpps_pad[comp->plane] *= 8;
//            bpp += bpps[comp->plane];
//            bpp_pad += bpps_pad[comp->plane];
//            bpps[comp->plane] >>= s;
//            bpps_pad[comp->plane] >>= s;
//        }
//        bpp >>= log2_pixels;
//        bpp_pad >>= log2_pixels;
//    }
//};

///*!
//    Assigns \a other to this VideoFormat implementation.
//*/
//VideoFormat& VideoFormat::operator=(const VideoFormat &other)
//{
//    d = other.d;
//    return *this;
//}

//VideoFormat& VideoFormat::operator =(VideoFormat::PixelFormat fmt)
//{
//    d->pixfmt = fmt;
//    d->init();
//    return *this;
//}

//VideoFormat& VideoFormat::operator =(QImage::Format qpixfmt)
//{
//    d->qpixfmt = qpixfmt;
//    d->init();
//    return *this;
//}

//VideoFormat& VideoFormat::operator =(int fffmt)
//{
//    d->pixfmt_ff = (AVPixelFormat)fffmt;
//    d->init();
//    return *this;
//}

//bool VideoFormat::operator==(const VideoFormat &other) const
//{
//    return d->pixfmt_ff == other.d->pixfmt_ff;
//}

//bool VideoFormat::operator==(VideoFormat::PixelFormat fmt) const
//{
//    return d->pixfmt == fmt;
//}

//bool VideoFormat::operator==(QImage::Format qpixfmt) const
//{
//    return d->qpixfmt == qpixfmt;
//}

//bool VideoFormat::operator==(int fffmt) const
//{
//    return d->pixfmt_ff == fffmt;
//}

//bool VideoFormat::operator!=(const VideoFormat& other) const
//{
//    return !(*this == other);
//}

//bool VideoFormat::operator!=(VideoFormat::PixelFormat fmt) const
//{
//    return d->pixfmt != fmt;
//}

//bool VideoFormat::operator!=(QImage::Format qpixfmt) const
//{
//    return d->qpixfmt != qpixfmt;
//}

//bool VideoFormat::operator!=(int fffmt) const
//{
//    return d->pixfmt_ff != fffmt;
//}

//bool VideoFormat::isValid() const
//{
//    return d->pixfmt_ff != QTAV_PIX_FMT_C(NONE) || d->pixfmt != Format_Invalid;
//}

//VideoFormat::PixelFormat VideoFormat::pixelFormat() const
//{
//    return d->pixfmt;
//}

//int VideoFormat::pixelFormatFFmpeg() const
//{
//    return d->pixfmt_ff;
//}

//QImage::Format VideoFormat::imageFormat() const
//{
//    return d->qpixfmt;
//}

//QString VideoFormat::name() const
//{
//    return d->name();
//}

//void VideoFormat::setPixelFormat(PixelFormat format)
//{
//    d->pixfmt = format;
//    d->init(format);
//}

//void VideoFormat::setPixelFormatFFmpeg(int format)
//{
//    d->pixfmt_ff = (AVPixelFormat)format;
//    d->init((AVPixelFormat)format);
//}

//int VideoFormat::channels() const
//{
//    if (!d->pixdesc)
//        return 0;
//    return d->pixdesc->nb_components;
//}

//int VideoFormat::planeCount() const
//{
//    return d->planes;
//}

//int VideoFormat::bitsPerPixel() const
//{
//    return d->bpp;
//}

//int VideoFormat::bitsPerPixelPadded() const
//{
//    return d->bpp_pad;
//}

//int VideoFormat::bitsPerPixelPadded(int plane) const
//{
//    if (plane >= d->bpps.size())
//        return 0;
//    return d->bpps_pad[plane];
//}

//int VideoFormat::bitsPerPixel(int plane) const
//{
//    //must be a valid index position in the vector
//    if (plane >= d->bpps.size())
//        return 0;
//    return d->bpps[plane];
//}

//int VideoFormat::bytesPerPixel() const
//{
//    return (bitsPerPixel() + 7) >> 3;
//}

//int VideoFormat::bytesPerPixel(int plane) const
//{
//    return (bitsPerPixel(plane) + 7) >> 3;
//}

//int VideoFormat::bytesPerLine(int width, int plane) const
//{
//    return d->bytesPerLine(width, plane);
//}

//int VideoFormat::chromaWidth(int lumaWidth) const
//{
//    return -((-lumaWidth) >> d->pixdesc->log2_chroma_w);
//}

//int VideoFormat::chromaHeight(int lumaHeight) const
//{
//    return -((-lumaHeight) >> d->pixdesc->log2_chroma_h);
//}

//// test AV_PIX_FMT_FLAG_XXX
//bool VideoFormat::isBigEndian() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_BE) == AV_PIX_FMT_FLAG_BE;
//}

//bool VideoFormat::hasPalette() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_PAL) == AV_PIX_FMT_FLAG_PAL;
//}

//bool VideoFormat::isPseudoPaletted() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_PSEUDOPAL) == AV_PIX_FMT_FLAG_PSEUDOPAL;
//}

//bool VideoFormat::isBitStream() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_BITSTREAM) == AV_PIX_FMT_FLAG_BITSTREAM;
//}

//bool VideoFormat::isHWAccelerated() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_HWACCEL) == AV_PIX_FMT_FLAG_HWACCEL;
//}

//bool VideoFormat::isPlanar() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_PLANAR) == AV_PIX_FMT_FLAG_PLANAR;
//}

//bool VideoFormat::isRGB() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_RGB) == AV_PIX_FMT_FLAG_RGB;
//}

//bool VideoFormat::hasAlpha() const
//{
//    return (d->flags() & AV_PIX_FMT_FLAG_ALPHA) == AV_PIX_FMT_FLAG_ALPHA;
//}

//bool VideoFormat::isPlanar(PixelFormat pixfmt)
//{
//    return pixfmt == Format_YUV420P || pixfmt == Format_NV12 || pixfmt == Format_NV21 || pixfmt == Format_YV12
//            || pixfmt == Format_YUV410P || pixfmt == Format_YUV411P || pixfmt == Format_YUV422P
//            || pixfmt == Format_YUV444P || pixfmt == Format_AYUV444
//        || pixfmt == Format_IMC1 || pixfmt == Format_IMC2 || pixfmt == Format_IMC3 || pixfmt == Format_IMC4
//            ;
//}

//bool VideoFormat::isRGB(PixelFormat pixfmt)
//{
//    return pixfmt == Format_RGB32 || pixfmt == Format_ARGB32
//        || pixfmt == Format_BGR24 || pixfmt == Format_BGRA32
//        || pixfmt == Format_BGR565 || pixfmt == Format_RGB555 || pixfmt == Format_RGB565
//        || pixfmt == Format_BGR24 || pixfmt == Format_BGR32 || pixfmt == Format_BGR555
//            ;
//}

//bool VideoFormat::hasAlpha(PixelFormat pixfmt)
//{
//    return pixfmt == Format_ARGB32 || pixfmt == Format_BGRA32
//        || pixfmt == Format_AYUV444// || pixfmt == Format_RGB555 || pixfmt == Format_BGR555
//            ;
//}


//#ifndef QT_NO_DEBUG_STREAM
//QDebug operator<<(QDebug dbg, const VideoFormat &fmt)
//{
//    dbg.nospace() << "QtAV::VideoFormat(pixelFormat: " << fmt.name();
//    dbg.nospace() << ", channels: " << fmt.channels();
//    dbg.nospace() << ", planes: " << fmt.planeCount();
//    dbg.nospace() << ", bitsPerPixel: " << fmt.bitsPerPixel();
//    dbg.nospace() << ")";
//    return dbg.space();
//}

//QDebug operator<<(QDebug dbg, VideoFormat::PixelFormat pixFmt)
//{
//    dbg.nospace() << av_get_pix_fmt_name((AVPixelFormat)VideoFormat::pixelFormatToFFmpeg(pixFmt));
//    return dbg.space();
//}
//#endif


//namespace {
//    class VideoFormatPrivateRegisterMetaTypes
//    {
//    public:
//        VideoFormatPrivateRegisterMetaTypes()
//        {
//            qRegisterMetaType<QtAV::VideoFormat>();
//            qRegisterMetaType<QtAV::VideoFormat::PixelFormat>();
//        }
//    } _registerMetaTypes;
//}

//} //namespace QtAV

