#ifndef I_MEDIA_STREAM_H
#define I_MEDIA_STREAM_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/master_clock.h"
#include <QMutex>

//#define FRAMES_LIMIT 25
//#define PACKETS_LIMIT 12

//#define FRAMES_LIMIT 30
//#define PACKETS_LIMIT 5

class IMediaStream {

public:
    IMediaStream(AVFormatContext * context, int streamIndex) : time_buff(0), packetsBufferLen(5), framesBufferLen(30) {
        stream = 0;
        codec_context = 0;
        codec = 0;
        frame = 0;
        mutex = 0;

        //TODO: emit error
        valid = !(streamIndex < 0 || streamIndex == AVERROR_STREAM_NOT_FOUND || streamIndex == AVERROR_DECODER_NOT_FOUND);
        if (!valid) return;

        uindex = streamIndex;

        stream = context -> streams[uindex];

        valid = stream != 0;
        if (!valid) return;

        is_attachment = stream -> disposition & AV_DISPOSITION_ATTACHED_PIC;

//        stream -> attached_pic
//            For streams with AV_DISPOSITION_ATTACHED_PIC disposition, this packet will contain the attached picture.

        codec_context = stream -> codec;
//        codec_context -> refcounted_frames = 1; // did not clear frames automatically
        codec = avcodec_find_decoder(codec_context -> codec_id);

//        //glitches with some video
//        if (codec -> capabilities & CODEC_CAP_TRUNCATED)
//            codec_context -> flags |= CODEC_FLAG_TRUNCATED;

//        int stream_lowres = ?;
        //    if(stream_lowres > av_codec_get_max_lowres(codec)){
        //        av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
        //                av_codec_get_max_lowres(codec));
        //        stream_lowres = av_codec_get_max_lowres(codec);
        //    }
//        av_codec_set_lowres(codec_context, stream_lowres);
//        if (stream_lowres) codec_context -> flags |= CODEC_FLAG_EMU_EDGE;

//        bool fast = true;
//        if (fast)
//          codec_context -> flags2 |= CODEC_FLAG2_FAST;

        if(codec -> capabilities & CODEC_CAP_DR1)
            codec_context -> flags |= CODEC_FLAG_EMU_EDGE;
        //////

        valid = codec != 0;
        if (!valid) return;

        is_lossless = codec -> capabilities & CODEC_CAP_LOSSLESS;
        is_vbr = codec -> capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE;
        is_planar = av_sample_fmt_is_planar(codec_context -> sample_fmt); /*&& codec_context -> channels > AV_NUM_DATA_POINTERS */

        AVDictionary *opts = NULL;

        if (codec_context -> codec_type == AVMEDIA_TYPE_VIDEO || codec_context -> codec_type == AVMEDIA_TYPE_AUDIO)
            av_dict_set(&opts, "refcounted_frames", "1", 0);

        //    opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);
        //    if (!av_dict_get(opts, "threads", NULL, 0))
        //        av_dict_set(&opts, "threads", "auto", 0);
        //    if (stream_lowres)
        //        av_dict_set(&opts, "lowres", av_asprintf("%d", stream_lowres), AV_DICT_DONT_STRDUP_VAL);
        //    if (avctx->codec_type == AVMEDIA_TYPE_VIDEO || avctx->codec_type == AVMEDIA_TYPE_AUDIO)
        //        av_dict_set(&opts, "refcounted_frames", "1", 0);
        //    if (avcodec_open2(avctx, codec, &opts) < 0)
        //        return -1;

        valid = avcodec_open2(codec_context, codec, &opts) > -1;
        av_dict_free(&opts);
        if (!valid) return;

        frame = av_frame_alloc();
        mutex = new QMutex();
    }

    ~IMediaStream() {
        delete mutex;

        if (frame)
            av_frame_free(&frame);

        foreach(AVPacket * pack, packets)
            av_free_packet(pack);

        packets.clear();

        if (codec_context) {
            avcodec_close(codec_context);
            av_freep(codec_context);
        }

        delete codec;
    }

    int calcDelay() {
        return time_buff == 0 ? 0 : time_buff * 60; // take only 6/10 from the time buffer for delay
    }

    inline int index() const { return uindex; }

    inline bool isBlocked() { return valid && packets.size() > packetsBufferLen; }
    inline bool isValid() const { return valid; }

    inline bool hasPackets() { return !packets.isEmpty(); }
    inline bool requirePreload() { return packets.isEmpty(); }

    void decode(AVPacket * newPacket) {
        mutex -> lock();
            packets.append(newPacket);
        mutex -> unlock();
    }

    void dropPackets() {
        if (!valid) return;
        mutex -> lock();
            while(packets.size() > 0)
                av_free_packet(packets.takeFirst());
            time_buff = 0;
        mutex -> unlock();
    }

protected:
    float time_buff;

    int packetsBufferLen;
    int framesBufferLen;

    bool valid;
    bool is_planar;
    bool is_attachment;
    bool is_lossless;
    bool is_vbr;

    QMutex * mutex;
    AVStream * stream;
    int uindex;
    AVCodecContext * codec_context;
    AVCodec * codec;
    AVFrame * frame;

    QList<AVPacket*> packets;
};

#endif // I_MEDIA_STREAM_H
