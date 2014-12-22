#ifndef VIDEO_BLENDING_H
#define VIDEO_BLENDING_H

#define ALPHA_BLEND(a, oldp, newp, s)\
((((oldp << s) * (255 - (a))) + (newp * (a))) / (255 << s))

/*#define RGBA_IN(r, g, b, a, s)\
{\
    unsigned int v = ((const uint32_t *)(s))[0];\
    a = (v >> 24) & 0xff;\
    r = (v >> 16) & 0xff;\
    g = (v >> 8) & 0xff;\
    b = v & 0xff;\
}*/

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

#include "media/media_player/utils/media_player_utils.h"
#include <QImage>

//TODO: not completed for rgb and not tested
class VideoBlending {
public:
    static void blend_subrect(QImage * /*image*/, const AVSubtitleRect * /*rect*/) {

    }

    static void blend_subrect(AVPicture * av_picture, int buff_width, int buff_height, const AVSubtitleRect *rect) {
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
};

#endif // VIDEO_BLENDING_H
