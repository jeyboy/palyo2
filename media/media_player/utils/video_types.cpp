#include "video_types.h"

struct fmt_entry {
    AVPixelFormat pixfmt;
    GLint internal_format;
    GLenum format;
    GLenum type;
};


//Internal Format     External Format Type                    Bytes per Pixel
//---------------     --------------- ----                    ---------------
//BGRA_EXT            BGRA_EXT        UNSIGNED_BYTE           4
//RGBA                RGBA            UNSIGNED_BYTE           4
//RGB                 RGB             UNSIGNED_BYTE           3
//RGBA                RGBA            UNSIGNED_SHORT_4_4_4_4  2
//RGBA                RGBA            UNSIGNED_SHORT_5_5_5_1  2
//RGB                 RGB             UNSIGNED_SHORT_5_6_5    2
//LUMINANCE_ALPHA     LUMINANCE_ALPHA UNSIGNED_BYTE           2
//LUMINANCE           LUMINANCE       UNSIGNED_BYTE           1
//ALPHA               ALPHA           UNSIGNED_BYTE           1

// TODO: maybe incorrect orders and types
static const struct fmt_entry pixfmt_to_gl_formats[] = {
//#define GL_UNSIGNED_BYTE_3_3_2            0x8032
//#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
//#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
//#define GL_UNSIGNED_INT_8_8_8_8           0x8035
//#define GL_UNSIGNED_INT_10_10_10_2        0x8036
//#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
//#define GL_UNSIGNED_SHORT_5_6_5           0x8363
//#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
//#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
//#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
//#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
//#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368



    #ifdef QT_OPENGL_ES_2
        {VideoTypes::PF_ARGB32, GL_BGRA, GL_BGRA, GL_UNSIGNED_BYTE },
        {VideoTypes::PF_RGB32,  GL_BGRA, GL_BGRA, GL_UNSIGNED_BYTE },
    #else
        {VideoTypes::PF_RGB32,  GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE },
        {VideoTypes::PF_ARGB32, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE },
    #endif

    {VideoTypes::PF_RGB24,  GL_RGB,  GL_RGB,  GL_UNSIGNED_BYTE },
//    {VideoFormat::Format_BGR24,  GL_RGB,  GL_BGR,  GL_UNSIGNED_BYTE },

    {VideoTypes::PF_RGB555LE, GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV},
    {VideoTypes::PF_RGB555BE, GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV},

    {VideoTypes::PF_BGR555LE, GL_BGRA, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV},
    {VideoTypes::PF_BGR555BE, GL_BGRA, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV},

    {VideoTypes::PF_RGB565LE, GL_RGB,  GL_RGB,  GL_UNSIGNED_SHORT_5_6_5},
    {VideoTypes::PF_RGB565BE, GL_RGB,  GL_RGB,  GL_UNSIGNED_SHORT_5_6_5_REV},

    {VideoTypes::PF_BGR565LE, GL_BGR,  GL_BGR,  GL_UNSIGNED_SHORT_5_6_5},
    {VideoTypes::PF_BGR565BE, GL_BGR,  GL_BGR,  GL_UNSIGNED_SHORT_5_6_5_REV},

    {VideoTypes::PF_BGRA32, GL_BGRA,  GL_RGBA,  GL_UNSIGNED_BYTE},

    //{VideoFormat::Format_BGRA32, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE },
    //{VideoFormat::Format_BGR32,  GL_BGRA, GL_BGRA, GL_UNSIGNED_BYTE },
};


static const struct {
    VideoTypes::PixelFormat fmt;
    AVPixelFormat ff;
    QImage::Format img_fmt;
} pix_fmt_map[] = {
    { VideoTypes::PF_Invalid,       AV_PIX_FMT_NONE,            QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    { VideoTypes::PF_YUV420P,       AV_PIX_FMT_YUV420P,         QImage::Format_Mono },

    ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV420P and setting color_range
    { VideoTypes::PF_YUV420P,       AV_PIX_FMT_YUVJ420P,        QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    { VideoTypes::PF_YV12,          AV_PIX_FMT_YUV420P,         QImage::Format_Invalid }, // ?

    ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    { VideoTypes::PF_YUYV,          AV_PIX_FMT_YUYV422,         QImage::Format_Invalid },

    ///< packed RGB 8:8:8, 24bpp, RGBRGB...
    { VideoTypes::PF_RGB24,         AV_PIX_FMT_RGB24,           QImage::Format_RGB888 },

    ///< packed RGB 8:8:8, 24bpp, BGRBGR...
    { VideoTypes::PF_BGR24,         AV_PIX_FMT_BGR24,           QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    { VideoTypes::PF_YUV422P,       AV_PIX_FMT_YUV422P,         QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV422P and setting color_range
    { VideoTypes::PF_YUV422P,       AV_PIX_FMT_YUVJ422P,        QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
    { VideoTypes::PF_YUV444P,       AV_PIX_FMT_YUV444P,         QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV444P and setting color_range
    { VideoTypes::PF_YUV444P,       AV_PIX_FMT_YUVJ444P,        QImage::Format_Invalid },

    ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
    { VideoTypes::PF_YUV410P,       AV_PIX_FMT_YUV410P,         QImage::Format_Invalid },

    ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
    { VideoTypes::PF_YUV411P,       AV_PIX_FMT_YUV411P,         QImage::Format_Invalid },


    //AV_PIX_FMT_GRAY8,     ///<        Y        ,  8bpp  // in future qt use Format_Grayscale8
    //AV_PIX_FMT_GRAY8A,    ///< 8bit gray, 8bit alpha
    //AV_PIX_FMT_GRAY16BE,  ///<        Y        , 16bpp, big-endian
    //AV_PIX_FMT_GRAY16LE,  ///<        Y        , 16bpp, little-endian

    ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
    { VideoTypes::PF_MONO,          AV_PIX_FMT_MONOWHITE,       QImage::Format_Mono },

    //QTAV_PIX_FMT_C(MONOBLACK), ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb

    ///< 8 bit with PIX_FMT_RGB32 palette
    { VideoTypes::PF_PAL8,          AV_PIX_FMT_PAL8,            QImage::Format_Indexed8 }, // NOT CHECKED

    ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
    { VideoTypes::PF_UYVY,          AV_PIX_FMT_UYVY422,         QImage::Format_Invalid },

    //AV_PIX_FMT_UYYVYY411, ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
    //AV_PIX_FMT_BGR8,      ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
    //AV_PIX_FMT_BGR4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
    //AV_PIX_FMT_BGR4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
    //AV_PIX_FMT_RGB8,      ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
    //AV_PIX_FMT_RGB4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
    //AV_PIX_FMT_RGB4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)

    ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
    { VideoTypes::PF_NV12,          AV_PIX_FMT_NV12,            QImage::Format_Invalid },

    ///< as above, but U and V bytes are swapped
    { VideoTypes::PF_NV21,          AV_PIX_FMT_NV21,            QImage::Format_Invalid },

    ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
    { VideoTypes::PF_ARGB32,        AV_PIX_FMT_ARGB,            QImage::Format_Invalid },

    { VideoTypes::PF_RGB32,         AV_PIX_FMT_RGB32,           QImage::Format_RGB32 }, //auto endian

    ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
    { VideoTypes::PF_RGB32,         AV_PIX_FMT_RGBA,            QImage::Format_RGBA8888 },

    //AV_PIX_FMT_ABGR,      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...

    ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...
    { VideoTypes::PF_BGRA32,        AV_PIX_FMT_BGRA,            QImage::Format_Invalid },


    //AV_PIX_FMT_YUV440P,   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
    //AV_PIX_FMT_YUVJ440P,  ///< planar YUV 4:4:0 full scale (JPEG), deprecated in favor of PIX_FMT_YUV440P and setting color_range
    //AV_PIX_FMT_YUVA420P,  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)

    //AV_PIX_FMT_RGB48BE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian
    //AV_PIX_FMT_RGB48LE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian

    ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
    { VideoTypes::PF_RGB565BE,      AV_PIX_FMT_RGB565BE,        QImage::Format_RGB16 },

    ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
    { VideoTypes::PF_RGB565LE,      AV_PIX_FMT_RGB565LE,        QImage::Format_Invalid },

    ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0
    { VideoTypes::PF_RGB555BE,      AV_PIX_FMT_RGB555BE,        QImage::Format_RGB555 },

    ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0
    { VideoTypes::PF_RGB555LE,      AV_PIX_FMT_RGB555LE,        QImage::Format_Invalid },

    ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
    { VideoTypes::PF_BGR565BE,      AV_PIX_FMT_BGR565BE,        QImage::Format_Invalid },

    ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
    { VideoTypes::PF_BGR565LE,      AV_PIX_FMT_BGR565LE,        QImage::Format_Invalid },

    ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), big-endian, most significant bit to 1
    { VideoTypes::PF_BGR555BE,      AV_PIX_FMT_BGR555BE,        QImage::Format_Invalid },

    ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), little-endian, most significant bit to 1
    { VideoTypes::PF_BGR555LE,      AV_PIX_FMT_BGR555LE,        QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    { VideoTypes::PF_YUV420P16LE,   AV_PIX_FMT_YUV420P16LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    { VideoTypes::PF_YUV420P16BE,   AV_PIX_FMT_YUV420P16BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    { VideoTypes::PF_YUV422P16LE,   AV_PIX_FMT_YUV422P16LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    { VideoTypes::PF_YUV422P16BE,   AV_PIX_FMT_YUV422P16BE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    { VideoTypes::PF_YUV444P16LE,   AV_PIX_FMT_YUV444P16LE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    { VideoTypes::PF_YUV444P16BE,   AV_PIX_FMT_YUV444P16BE,     QImage::Format_Invalid },

    ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), little-endian, most significant bits to 0
    { VideoTypes::PF_RGB444LE,      AV_PIX_FMT_RGB444LE,        QImage::Format_Invalid },

    ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), big-endian, most significant bits to 0
    { VideoTypes::PF_RGB444BE,      AV_PIX_FMT_RGB444BE,        QImage::Format_RGB444 },



    //AV_PIX_FMT_BGR444LE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), little-endian, most significant bits to 1
    //AV_PIX_FMT_BGR444BE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), big-endian, most significant bits to 1

    //AV_PIX_FMT_BGR48BE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian
    //AV_PIX_FMT_BGR48LE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian

    /**
     * The following 12 formats have the disadvantage of needing 1 format for each bit depth.
     * Notice that each 9/10 bits sample is stored in 16 bits with extra padding.
     * If you want to support multiple bit depths, then using AV_PIX_FMT_YUV420P16* with the bpp stored separately is better.
     */

    ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    { VideoTypes::PF_YUV420P9BE,    AV_PIX_FMT_YUV420P9BE,      QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    { VideoTypes::PF_YUV420P9LE,    AV_PIX_FMT_YUV420P9LE,      QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    { VideoTypes::PF_YUV420P10BE,   AV_PIX_FMT_YUV420P10BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    { VideoTypes::PF_YUV420P10LE,   AV_PIX_FMT_YUV420P10LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    { VideoTypes::PF_YUV422P10BE,   AV_PIX_FMT_YUV422P10BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    { VideoTypes::PF_YUV422P10LE,   AV_PIX_FMT_YUV422P10LE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    { VideoTypes::PF_YUV444P9BE,    AV_PIX_FMT_YUV444P9BE,      QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    { VideoTypes::PF_YUV444P9LE,    AV_PIX_FMT_YUV444P9LE,      QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    { VideoTypes::PF_YUV444P10BE,   AV_PIX_FMT_YUV444P10BE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    { VideoTypes::PF_YUV444P10LE,   AV_PIX_FMT_YUV444P10LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    { VideoTypes::PF_YUV422P9BE,    AV_PIX_FMT_YUV422P9BE,      QImage::Format_Invalid },

    ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    { VideoTypes::PF_YUV422P9LE,    AV_PIX_FMT_YUV422P9LE,      QImage::Format_Invalid },


    #ifdef QTAV_PIX_FMT_C(ABI_GIT_MASTER
        //AV_PIX_FMT_RGBA64BE,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
        //AV_PIX_FMT_RGBA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
        //AV_PIX_FMT_BGRA64BE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
        //AV_PIX_FMT_BGRA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
    #endif
    //AV_PIX_FMT_GBRP,      ///< planar GBR 4:4:4 24bpp
    //AV_PIX_FMT_GBRP9BE,   ///< planar GBR 4:4:4 27bpp, big-endian
    //AV_PIX_FMT_GBRP9LE,   ///< planar GBR 4:4:4 27bpp, little-endian
    //AV_PIX_FMT_GBRP10BE,  ///< planar GBR 4:4:4 30bpp, big-endian
    //AV_PIX_FMT_GBRP10LE,  ///< planar GBR 4:4:4 30bpp, little-endian
    //AV_PIX_FMT_GBRP16BE,  ///< planar GBR 4:4:4 48bpp, big-endian
    //AV_PIX_FMT_GBRP16LE,  ///< planar GBR 4:4:4 48bpp, little-endian

    /**
     * duplicated pixel formats for compatibility with libav.
     * FFmpeg supports these formats since May 8 2012 and Jan 28 2012 (commits f9ca1ac7 and 143a5c55)
     * Libav added them Oct 12 2012 with incompatible values (commit 6d5600e85)
     */

    //AV_PIX_FMT_YUVA422P_LIBAV,  ///< planar YUV 4:2:2 24bpp, (1 Cr & Cb sample per 2x1 Y & A samples)
    //AV_PIX_FMT_YUVA444P_LIBAV,  ///< planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)

    //AV_PIX_FMT_YUVA420P9BE,  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), big-endian
    //AV_PIX_FMT_YUVA420P9LE,  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), little-endian
    //AV_PIX_FMT_YUVA422P9BE,  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), big-endian
    //AV_PIX_FMT_YUVA422P9LE,  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), little-endian
    //AV_PIX_FMT_YUVA444P9BE,  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), big-endian
    //AV_PIX_FMT_YUVA444P9LE,  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
    //AV_PIX_FMT_YUVA420P10BE, ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
    //AV_PIX_FMT_YUVA420P10LE, ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
    //AV_PIX_FMT_YUVA422P10BE, ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
    //AV_PIX_FMT_YUVA422P10LE, ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
    //AV_PIX_FMT_YUVA444P10BE, ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
    //AV_PIX_FMT_YUVA444P10LE, ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)
    //AV_PIX_FMT_YUVA420P16BE, ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
    //AV_PIX_FMT_YUVA420P16LE, ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
    //AV_PIX_FMT_YUVA422P16BE, ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
    //AV_PIX_FMT_YUVA422P16LE, ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
    //AV_PIX_FMT_YUVA444P16BE, ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
    //AV_PIX_FMT_YUVA444P16LE, ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)

    //AV_PIX_FMT_XYZ12LE,      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as little-endian, the 4 lower bits are set to 0
    //AV_PIX_FMT_XYZ12BE,      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as big-endian, the 4 lower bits are set to 0
    //AV_PIX_FMT_NV16,         ///< interleaved chroma YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    //AV_PIX_FMT_NV20LE,       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    //AV_PIX_FMT_NV20BE,       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian

    /**
     * duplicated pixel formats for compatibility with libav.
     * FFmpeg supports these formats since Sat Sep 24 06:01:45 2011 +0200 (commits 9569a3c9f41387a8c7d1ce97d8693520477a66c3)
     * also see Fri Nov 25 01:38:21 2011 +0100 92afb431621c79155fcb7171d26f137eb1bee028
     * Libav added them Sun Mar 16 23:05:47 2014 +0100 with incompatible values (commit 1481d24c3a0abf81e1d7a514547bd5305232be30)
     */
    //AV_PIX_FMT_RGBA64BE_LIBAV,     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
    //AV_PIX_FMT_RGBA64LE_LIBAV,     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
    //AV_PIX_FMT_BGRA64BE_LIBAV,     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
    //AV_PIX_FMT_BGRA64LE_LIBAV,     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian

    //AV_PIX_FMT_YVYU422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cr Y1 Cb

    //AV_PIX_FMT_VDA,          ///< HW acceleration through VDA, data[3] contains a CVPixelBufferRef

    //AV_PIX_FMT_YA16BE,       ///< 16bit gray, 16bit alpha (big-endian)
    //AV_PIX_FMT_YA16LE,       ///< 16bit gray, 16bit alpha (little-endian)



    #ifndef QTAV_PIX_FMT_C(ABI_GIT_MASTER
        //AV_PIX_FMT_RGBA64BE=0x123,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
        //AV_PIX_FMT_RGBA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
        //AV_PIX_FMT_BGRA64BE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
        //AV_PIX_FMT_BGRA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
    #endif

    ///< packed RGB 8:8:8, 32bpp, 0RGB0RGB...
    { VideoTypes::PF_ARGB32,        AV_PIX_FMT_0RGB,            QImage::Format_ARGB32 },

    ///< packed RGB 8:8:8, 32bpp, RGB0RGB0...
    { VideoTypes::PF_RGB32,         AV_PIX_FMT_RGB0,            QImage::Format_ARGB32 },

    //AV_PIX_FMT_0BGR,      ///< packed BGR 8:8:8, 32bpp, 0BGR0BGR...
    //AV_PIX_FMT_BGR0,      ///< packed BGR 8:8:8, 32bpp, BGR0BGR0...
    //AV_PIX_FMT_YUVA444P,  ///< planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)
    //AV_PIX_FMT_YUVA422P,  ///< planar YUV 4:2:2 24bpp, (1 Cr & Cb sample per 2x1 Y & A samples)

    ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    { VideoTypes::PF_YUV420P12BE,   AV_PIX_FMT_YUV420P12BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    { VideoTypes::PF_YUV420P12LE,   AV_PIX_FMT_YUV420P12LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    { VideoTypes::PF_YUV420P14BE,   AV_PIX_FMT_YUV420P14BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    { VideoTypes::PF_YUV420P14LE,   AV_PIX_FMT_YUV420P14LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    { VideoTypes::PF_YUV422P12BE,   AV_PIX_FMT_YUV422P12BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    { VideoTypes::PF_YUV422P12LE,   AV_PIX_FMT_YUV422P12LE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    { VideoTypes::PF_YUV422P14BE,   AV_PIX_FMT_YUV422P14BE,     QImage::Format_Invalid },

    ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    { VideoTypes::PF_YUV422P14LE,   AV_PIX_FMT_YUV422P14LE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    { VideoTypes::PF_YUV444P12BE,   AV_PIX_FMT_YUV444P12BE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    { VideoTypes::PF_YUV444P12LE,   AV_PIX_FMT_YUV444P12LE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    { VideoTypes::PF_YUV444P14BE,   AV_PIX_FMT_YUV444P14BE,     QImage::Format_Invalid },

    ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    { VideoTypes::PF_YUV444P14LE,   AV_PIX_FMT_YUV444P14LE,     QImage::Format_Invalid },

    //AV_PIX_FMT_GBRP12BE,    ///< planar GBR 4:4:4 36bpp, big-endian
    //AV_PIX_FMT_GBRP12LE,    ///< planar GBR 4:4:4 36bpp, little-endian
    //AV_PIX_FMT_GBRP14BE,    ///< planar GBR 4:4:4 42bpp, big-endian
    //AV_PIX_FMT_GBRP14LE,    ///< planar GBR 4:4:4 42bpp, little-endian
    //AV_PIX_FMT_GBRAP,       ///< planar GBRA 4:4:4:4 32bpp
    //AV_PIX_FMT_GBRAP16BE,   ///< planar GBRA 4:4:4:4 64bpp, big-endian
    //AV_PIX_FMT_GBRAP16LE,   ///< planar GBRA 4:4:4:4 64bpp, little-endian
    //AV_PIX_FMT_YUVJ411P,    ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples) full scale (JPEG), deprecated in favor of PIX_FMT_YUV411P and setting color_range

    //AV_PIX_FMT_BAYER_BGGR8,    ///< bayer, BGBG..(odd line), GRGR..(even line), 8-bit samples
    //AV_PIX_FMT_BAYER_RGGB8,    ///< bayer, RGRG..(odd line), GBGB..(even line), 8-bit samples
    //AV_PIX_FMT_BAYER_GBRG8,    ///< bayer, GBGB..(odd line), RGRG..(even line), 8-bit samples
    //AV_PIX_FMT_BAYER_GRBG8,    ///< bayer, GRGR..(odd line), BGBG..(even line), 8-bit samples
    //AV_PIX_FMT_BAYER_BGGR16LE, ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, little-endian
    //AV_PIX_FMT_BAYER_BGGR16BE, ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, big-endian
    //AV_PIX_FMT_BAYER_RGGB16LE, ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, little-endian
    //AV_PIX_FMT_BAYER_RGGB16BE, ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, big-endian
    //AV_PIX_FMT_BAYER_GBRG16LE, ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, little-endian
    //AV_PIX_FMT_BAYER_GBRG16BE, ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, big-endian
    //AV_PIX_FMT_BAYER_GRBG16LE, ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, little-endian
    //AV_PIX_FMT_BAYER_GRBG16BE, ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, big-endian
};

bool VideoTypes::isCompatible(AVPixelFormat & fmt) {
    for (unsigned int i = 0; i < sizeof(pix_fmt_map)/sizeof(pix_fmt_map[0]); ++i) {
        if (pix_fmt_map[i].ff == fmt)
            return true;
    }

    return false;
}

QImage::Format VideoTypes::toQImageFormat(AVPixelFormat & fmt) {
    for (unsigned int i = 0; i < sizeof(pix_fmt_map)/sizeof(pix_fmt_map[0]); ++i) {
        if (pix_fmt_map[i].ff == fmt)
            return pix_fmt_map[i].img_fmt;
    }

    default: return QImage::Format_Invalid;
}

PixelFormat VideoTypes::toPixelFormat(AVPixelFormat & fmt) {
    for (unsigned int i = 0; i < sizeof(pix_fmt_map)/sizeof(pix_fmt_map[0]); ++i) {
        if (pix_fmt_map[i].ff == fmt)
            return pix_fmt_map[i].fmt;
    }
    return VideoTypes::PF_Invalid;
}

bool VideoTypes::videoFormatToGL(const AVPixelFormat & fmt, GLint* internal_format, GLenum* data_format, GLenum* data_type) {
    for (unsigned int i = 0; i < sizeof(pixfmt_to_gl_formats)/sizeof(pixfmt_to_gl_formats[0]); ++i) {
        if (pixfmt_to_gl_formats[i].pixfmt == fmt.pixelFormat()) {
            *internal_format = pixfmt_to_gl_formats[i].internal_format;
            *data_format = pixfmt_to_gl_formats[i].format;
            *data_type = pixfmt_to_gl_formats[i].type;
            return true;
        }
    }
    return false;
}


//https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_format_BGRA8888.txt
int VideoTypes::bytesPerGLFormat(GLenum format, GLenum dataType) {
    switch (format) {
        #ifdef GL_BGRA //ifndef GL_ES
            case GL_BGRA:
        #endif

        case GL_RGBA:
            return 4;

        #ifdef GL_BGR //ifndef GL_ES
            case GL_BGR:
        #endif

        case GL_RGB:
            switch (dataType) {
                case GL_UNSIGNED_SHORT_5_6_5:
                    return 2;
                default:
                    return 3;
            }
//            return 3;
        case GL_LUMINANCE_ALPHA:
            return 2;
        case GL_LUMINANCE:
        case GL_ALPHA:
                return 1;

        #ifdef GL_LUMINANCE16
            case GL_LUMINANCE16:
                return 2;
        #endif //GL_LUMINANCE16

        #ifdef GL_ALPHA16
            case GL_ALPHA16:
                return 2;
        #endif //GL_ALPHA16

        default:
            qWarning("bytesOfGLFormat - Unknown format %u", format);
            return 1;
    }
}

GLint VideoTypes::getGLInternalFormat(GLint data_format, int bpp) {
    if (bpp != 2)
        return data_format;

    switch (data_format) {
        #ifdef GL_ALPHA16
            case GL_ALPHA:
                return GL_ALPHA16;
        #endif //GL_ALPHA16

        #ifdef GL_LUMINANCE16
            case GL_LUMINANCE:
                return GL_LUMINANCE16;
        #endif //GL_LUMINANCE16

        default:
            return data_format;
    }
}
