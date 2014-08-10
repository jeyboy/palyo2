#ifndef SUBTITLE_STREAM_H
#define SUBTITLE_STREAM_H

#include "media/media_player/streams/media_stream.h"

class SubtitleStream : public MediaStream {
public:
    SubtitleStream(QObject * parent, AVFormatContext * context, int streamIndex, Priority priority = InheritPriority);

    void suspendOutput();
    void resumeOutput();
protected:
    void routine();
};

#endif // SUBTITLE_STREAM_H


//static int subtitle_thread(void *arg)
//{
//    VideoState *is = arg;
//    SubPicture *sp;
//    AVPacket pkt1, *pkt = &pkt1;
//    int got_subtitle;
//    int serial;
//    double pts;
//    int i, j;
//    int r, g, b, y, u, v, a;

//    for (;;) {
//        while (is->paused && !is->subtitleq.abort_request) {
//            SDL_Delay(10);
//        }
//        if (packet_queue_get(&is->subtitleq, pkt, 1, &serial) < 0)
//            break;

//        if (pkt->data == flush_pkt.data) {
//            avcodec_flush_buffers(is->subtitle_st->codec);
//            continue;
//        }
//        SDL_LockMutex(is->subpq_mutex);
//        while (is->subpq_size >= SUBPICTURE_QUEUE_SIZE &&
//               !is->subtitleq.abort_request) {
//            SDL_CondWait(is->subpq_cond, is->subpq_mutex);
//        }
//        SDL_UnlockMutex(is->subpq_mutex);

//        if (is->subtitleq.abort_request)
//            return 0;

//        sp = &is->subpq[is->subpq_windex];

//       /* NOTE: ipts is the PTS of the _first_ picture beginning in
//           this packet, if any */
//        pts = 0;
//        if (pkt->pts != AV_NOPTS_VALUE)
//            pts = av_q2d(is->subtitle_st->time_base) * pkt->pts;

//        avcodec_decode_subtitle2(is->subtitle_st->codec, &sp->sub,
//                                 &got_subtitle, pkt);
//        if (got_subtitle && sp->sub.format == 0) {
//            if (sp->sub.pts != AV_NOPTS_VALUE)
//                pts = sp->sub.pts / (double)AV_TIME_BASE;
//            sp->pts = pts;
//            sp->serial = serial;

//            for (i = 0; i < sp->sub.num_rects; i++)
//            {
//                for (j = 0; j < sp->sub.rects[i]->nb_colors; j++)
//                {
//                    RGBA_IN(r, g, b, a, (uint32_t*)sp->sub.rects[i]->pict.data[1] + j);
//                    y = RGB_TO_Y_CCIR(r, g, b);
//                    u = RGB_TO_U_CCIR(r, g, b, 0);
//                    v = RGB_TO_V_CCIR(r, g, b, 0);
//                    YUVA_OUT((uint32_t*)sp->sub.rects[i]->pict.data[1] + j, y, u, v, a);
//                }
//            }

//            /* now we can update the picture count */
//            if (++is->subpq_windex == SUBPICTURE_QUEUE_SIZE)
//                is->subpq_windex = 0;
//            SDL_LockMutex(is->subpq_mutex);
//            is->subpq_size++;
//            SDL_UnlockMutex(is->subpq_mutex);
//        } else if (got_subtitle) {
//            avsubtitle_free(&sp->sub);
//        }
//        av_free_packet(pkt);
//    }
//    return 0;
//}