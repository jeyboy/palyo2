#ifndef VIDEO_TYPES_H
#define VIDEO_TYPES_H

#include "media_player_utils.h"
#include <QImage>

//#include <GL/gl.h>
//#include <GL/glext.h>
#include <QtOpenGL>

struct VideoTypes {
    enum VideoType {
        PF_Invalid = -1,

        PF_MONO,
        PF_PAL8,

        PF_ARGB32,
        PF_RGB32,
        PF_RGB24,
        PF_RGB565BE,
        PF_RGB565LE,
        PF_RGB555BE,
        PF_RGB555LE,
        PF_BGRA32,
        PF_BGR32,
        PF_BGR24,
        PF_BGR565BE,
        PF_BGR565LE,
        PF_BGR555BE,
        PF_BGR555LE,
        PF_RGB444BE,
        PF_RGB444LE,

        //http://www.fourcc.org/yuv.php
        PF_AYUV444,
        PF_YUV444P,
        PF_YUV422P,
        PF_YUV420P,
        PF_YUV411P,
        PF_YUV410P,
        PF_YV12,
        PF_UYVY,
        PF_YUYV,
        PF_NV12,
        PF_NV21,
        PF_IMC1,
        PF_IMC2,
        PF_IMC3,
        PF_IMC4,
        PF_Y8,
        PF_Y16,

//        Format_Jpeg,

//        Format_CameraRaw,
//        Format_AdobeDng,

        PF_YUV420P9LE,
        PF_YUV422P9LE,
        PF_YUV444P9LE,
        PF_YUV420P10LE,
        PF_YUV422P10LE,
        PF_YUV444P10LE,
        PF_YUV420P12LE,
        PF_YUV422P12LE,
        PF_YUV444P12LE,
        PF_YUV420P14LE,
        PF_YUV422P14LE,
        PF_YUV444P14LE,
        PF_YUV420P16LE,
        PF_YUV422P16LE,
        PF_YUV444P16LE,
        PF_YUV420P9BE,
        PF_YUV422P9BE,
        PF_YUV444P9BE,
        PF_YUV420P10BE,
        PF_YUV422P10BE,
        PF_YUV444P10BE,
        PF_YUV420P12BE,
        PF_YUV422P12BE,
        PF_YUV444P12BE,
        PF_YUV420P14BE,
        PF_YUV422P14BE,
        PF_YUV444P14BE,
        PF_YUV420P16BE,
        PF_YUV422P16BE,
        PF_YUV444P16BE,
//        Format_User
    };

    static bool isCompatible(AVPixelFormat & fmt);
    static QImage::Format toQImageFormat(AVPixelFormat & fmt);
    static VideoTypes::VideoType toVideoType(AVPixelFormat & fmt);

    static bool videoFormatToGL(const VideoType type, GLint* internal_format, GLenum* data_format, GLenum* data_type);
    static int bytesPerGLFormat(GLenum format, GLenum dataType);
    static GLint getGLInternalFormat(GLint data_format, int bpp);
};

#endif // VIDEO_TYPES_H
