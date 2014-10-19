//        QString str(av_get_pix_fmt_name(fmt));
//        QRegExp rgb_pattern("(RGB)|(BGR)");
//        qDebug() << "BBBBBBBB " << str << " " << new QString(av_get_pix_fmt_name(fmt) << 10);

#ifndef VIDEO_BUFFER_H
#define VIDEO_BUFFER_H

#include <QBitmap>
#include "../utils/media_player_utils.h"

#define ALPHA_BLEND(a, oldp, newp, s)\
((((oldp << s) * (255 - (a))) + (newp * (a))) / (255 << s))

//#define RGBA_IN(r, g, b, a, s)\
//{\
//    unsigned int v = ((const uint32_t *)(s))[0];\
//    a = (v >> 24) & 0xff;\
//    r = (v >> 16) & 0xff;\
//    g = (v >> 8) & 0xff;\
//    b = v & 0xff;\
//}

#define YUVA_IN(y, u, v, a, s, pal)\
{\
    unsigned int val = ((const uint32_t *)(pal))[*(const uint8_t*)(s)];\
    a = (val >> 24) & 0xff;\
    y = (val >> 16) & 0xff;\
    u = (val >> 8) & 0xff;\
    v = val & 0xff;\
}

#define YUVA_OUT(d, y, u, v, a)\
{\
    ((uint32_t *)(d))[0] = (a << 24) | (y << 16) | (u << 8) | v;\
}

#define BPP 1

class VideoBuffer {
public:
    enum VideoBufferType {
        other,
        rgb
    };

    VideoBuffer(QImage * img, int width, int height, AVPixelFormat format, AVColorSpace colorspace) : av_picture(0) {
        color_space = colorspace;
        fmt = format;
        buff_width = width;
        buff_height = height;
        q_image = img;
        descriptor = av_pix_fmt_desc_get(fmt);
    }

    VideoBuffer(AVPicture * pict, int width, int height, AVPixelFormat format, AVColorSpace colorspace) : q_image(0) {
        color_space = colorspace;
        fmt = format;
        buff_width = width;
        buff_height = height;
        av_picture = pict;
        descriptor = av_pix_fmt_desc_get(fmt);
    }

    ~VideoBuffer() {
        if (q_image)
            delete q_image;
        if (av_picture)
            avpicture_free(av_picture);

        av_freep(descriptor);
    }

    QImage * asQImage() { return q_image; }
    AVPicture * asAVPicture() { return av_picture; }

    int channelsCount() const {
        AVPixFmtDescriptor * descriptor = av_pix_fmt_desc_get(fmt);
        int res = descriptor -> nb_components;
        delete descriptor;
        return res;
    }

    //TODO: not completed for rgb and not tested
    void blend_subrect(const AVSubtitleRect *rect) {
        if (q_image)
            rgba_blend_subrect(rect);
        else
            yuva_blend_subrect(rect);
    }

    enum VideoBufferType type() const {
        if (q_image) {
            return rgb;
        } else return other;
    }

    bool isRGB() { return (descriptor -> flags & AV_PIX_FMT_FLAG_RGB) == AV_PIX_FMT_FLAG_RGB; }
    bool isHWACCEL() { return (descriptor -> flags & AV_PIX_FMT_FLAG_HWACCEL) == AV_PIX_FMT_FLAG_HWACCEL; }
    bool isPlanar() { return (descriptor -> flags & AV_PIX_FMT_FLAG_PLANAR) == AV_PIX_FMT_FLAG_PLANAR; }
    bool isBigEndian() { return (descriptor -> flags & AV_PIX_FMT_FLAG_BE) == AV_PIX_FMT_FLAG_BE; }
    bool isBitstream() { return (descriptor -> flags & AV_PIX_FMT_FLAG_BITSTREAM) == AV_PIX_FMT_FLAG_BITSTREAM; }
    bool hasPalette() { return (descriptor -> flags & AV_PIX_FMT_FLAG_PAL) == AV_PIX_FMT_FLAG_PAL; }
    bool hasPseudoPalette() { return (descriptor -> flags & AV_PIX_FMT_FLAG_PSEUDOPAL) == AV_PIX_FMT_FLAG_PSEUDOPAL; }
    bool hasAlpha() { return (descriptor -> flags & AV_PIX_FMT_FLAG_ALPHA) == AV_PIX_FMT_FLAG_ALPHA; }







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

protected:
    void rgba_blend_subrect(const AVSubtitleRect *rect) {

    }

    void yuva_blend_subrect(const AVSubtitleRect *rect) {
        int wrap, wrap3, width2, skip2;
        int y, u, v, a, u1, v1, a1, w, h;
        uint8_t *lum, *cb, *cr;
        const uint8_t *p;
        const uint32_t *pal;
        int dstx, dsty, dstw, dsth;

        dstw = av_clip(rect -> w, 0, buff_width);
        dsth = av_clip(rect -> h, 0, buff_height);
        dstx = av_clip(rect -> x, 0, buff_width - dstw);
        dsty = av_clip(rect -> y, 0, buff_height - dsth);
        lum = av_picture -> data[0] + dsty * av_picture -> linesize[0];
        cb  = av_picture -> data[1] + (dsty >> 1) * av_picture -> linesize[1];
        cr  = av_picture -> data[2] + (dsty >> 1) * av_picture -> linesize[2];

        width2 = ((dstw + 1) >> 1) + (dstx & ~dstw & 1);
        skip2 = dstx >> 1;
        wrap = av_picture -> linesize[0];
        wrap3 = rect -> pict.linesize[0];
        p = rect -> pict.data[0];
        pal = (const uint32_t *)rect -> pict.data[1];  /* Now in YCrCb! */

        if (dsty & 1) {
            lum += dstx;
            cb += skip2;
            cr += skip2;

            if (dstx & 1) {
                YUVA_IN(y, u, v, a, p, pal);
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
                cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
                cb++;
                cr++;
                lum++;
                p += BPP;
            }
            for (w = dstw - (dstx & 1); w >= 2; w -= 2) {
                YUVA_IN(y, u, v, a, p, pal);
                u1 = u;
                v1 = v;
                a1 = a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

                YUVA_IN(y, u, v, a, p + BPP, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
                cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
                cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
                cb++;
                cr++;
                p += 2 * BPP;
                lum += 2;
            }
            if (w) {
                YUVA_IN(y, u, v, a, p, pal);
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
                cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
                p++;
                lum++;
            }
            p += wrap3 - dstw * BPP;
            lum += wrap - dstw - dstx;
            cb += av_picture -> linesize[1] - width2 - skip2;
            cr += av_picture -> linesize[2] - width2 - skip2;
        }
        for (h = dsth - (dsty & 1); h >= 2; h -= 2) {
            lum += dstx;
            cb += skip2;
            cr += skip2;

            if (dstx & 1) {
                YUVA_IN(y, u, v, a, p, pal);
                u1 = u;
                v1 = v;
                a1 = a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                p += wrap3;
                lum += wrap;
                YUVA_IN(y, u, v, a, p, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
                cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
                cb++;
                cr++;
                p += -wrap3 + BPP;
                lum += -wrap + 1;
            }
            for (w = dstw - (dstx & 1); w >= 2; w -= 2) {
                YUVA_IN(y, u, v, a, p, pal);
                u1 = u;
                v1 = v;
                a1 = a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

                YUVA_IN(y, u, v, a, p + BPP, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
                p += wrap3;
                lum += wrap;

                YUVA_IN(y, u, v, a, p, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

                YUVA_IN(y, u, v, a, p + BPP, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[1] = ALPHA_BLEND(a, lum[1], y, 0);

                cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 2);
                cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 2);

                cb++;
                cr++;
                p += -wrap3 + 2 * BPP;
                lum += -wrap + 2;
            }
            if (w) {
                YUVA_IN(y, u, v, a, p, pal);
                u1 = u;
                v1 = v;
                a1 = a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                p += wrap3;
                lum += wrap;
                YUVA_IN(y, u, v, a, p, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
                cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
                cb++;
                cr++;
                p += -wrap3 + BPP;
                lum += -wrap + 1;
            }
            p += wrap3 + (wrap3 - dstw * BPP);
            lum += wrap + (wrap - dstw - dstx);
            cb += av_picture -> linesize[1] - width2 - skip2;
            cr += av_picture -> linesize[2] - width2 - skip2;
        }
        /* handle odd height */
        if (h) {
            lum += dstx;
            cb += skip2;
            cr += skip2;

            if (dstx & 1) {
                YUVA_IN(y, u, v, a, p, pal);
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
                cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
                cb++;
                cr++;
                lum++;
                p += BPP;
            }
            for (w = dstw - (dstx & 1); w >= 2; w -= 2) {
                YUVA_IN(y, u, v, a, p, pal);
                u1 = u;
                v1 = v;
                a1 = a;
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

                YUVA_IN(y, u, v, a, p + BPP, pal);
                u1 += u;
                v1 += v;
                a1 += a;
                lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
                cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u, 1);
                cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v, 1);
                cb++;
                cr++;
                p += 2 * BPP;
                lum += 2;
            }
            if (w) {
                YUVA_IN(y, u, v, a, p, pal);
                lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
                cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
                cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
            }
        }
    }

private:
    int buff_width, buff_height;
    AVPixelFormat fmt;
    VideoBufferType buff_type;
    QImage * q_image;
    AVPicture * av_picture;
    AVColorSpace color_space;
    AVPixFmtDescriptor * descriptor;
};

#endif // VIDEO_BUFFER_H


//#ifndef VIDEO_BUFFER_H
//#define VIDEO_BUFFER_H

//#include <QImage>

//struct VideoBuffer {
//    VideoBuffer(void * data, QList<QSize>) {

//    }

//    ~VideoBuffer() {
//        QImage * img = asImage();
//        if (img != 0)
//            delete asImage();
//        else {
//            uint8_t * buffers = asBuffers();
//            av_freep((uint8_t *)buffers[0]);
//            av_freep((uint8_t *)buffers[1]);
//            av_freep((uint8_t *)buffers[2]);
//            av_freep(buffers);
//        }
//    }

//    QImage * asImage() { return (QImage *)image; }
//    uint8_t * asBuffers() { return (uint8_t *)image; }

//    void * image;

//};

//#endif // VIDEO_BUFFER_H
