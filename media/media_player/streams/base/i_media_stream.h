#ifndef I_MEDIA_STREAM_H
#define I_MEDIA_STREAM_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/master_clock.h"
#include <QMutex>

#define FRAMES_LIMIT 16
#define PACKETS_LIMIT 8


class IMediaStream {

public:
    IMediaStream(AVFormatContext * context, int streamIndex) {
        pause = false;
        valid = true;
        stream = 0;
        codec_context = 0;
        codec = 0;
        frame = 0;
        mutex = 0;

        if (streamIndex < 0 || streamIndex == AVERROR_STREAM_NOT_FOUND || streamIndex == AVERROR_DECODER_NOT_FOUND) {
            valid = false;
            return;
        }

        uindex = streamIndex;

        stream = context -> streams[uindex];

        if (stream == 0 || stream -> disposition & AV_DISPOSITION_ATTACHED_PIC) { // block attachments picts
            valid = false;
            return;
        }

        codec_context = stream -> codec;
        codec = avcodec_find_decoder(codec_context -> codec_id);

//        //wtf
//        if (codec -> capabilities & CODEC_CAP_TRUNCATED)
//            codec_context -> flags |= CODEC_FLAG_TRUNCATED;

//        codec_context -> flags |= CODEC_FLAG_EMU_EDGE;
//        codec_context -> flags2 |= CODEC_FLAG2_FAST;
//        if(codec -> capabilities & CODEC_CAP_DR1)
//            codec_context -> flags |= CODEC_FLAG_EMU_EDGE;


    //    if(codec -> capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE)
    //        //Audio encoder supports receiving a different number of samples in each call.

    //    if(codec -> capabilities & CODEC_CAP_LOSSLESS)
    //        //Codec is lossless.

        //////

        if (codec == 0) {
            valid = false;
            return;
        }


        AVDictionary *opts = NULL;

        if (codec_context -> codec_type == AVMEDIA_TYPE_VIDEO || codec_context -> codec_type == AVMEDIA_TYPE_AUDIO)
            av_dict_set(&opts, "refcounted_frames", "1", 0);

        if (avcodec_open2(codec_context, codec, &opts) < 0) {
            valid = false;
            return;
        }

        av_dict_free(&opts);
        frame = av_frame_alloc();
        mutex = new QMutex();
    }

    ~IMediaStream() {
        delete mutex;

        if (frame)
            av_frame_free(&frame);

        foreach(AVPacket * pack, packets)
            av_free_packet(pack);

        avcodec_close(codec_context);
//        delete codec_context;

        delete codec; // hz
    }

    inline bool isBlocked() { return valid && packets.size() > PACKETS_LIMIT; }
    inline bool isValid() const { return valid; }
    inline int index() const { return uindex; }
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
        mutex -> unlock();
    }

    bool seeking(AVFormatContext * context, int64_t target, int flags) {
        if (!valid) return false;

        target = av_rescale_q(target, AV_TIME_BASE_Q, context -> streams[uindex] -> time_base);
        return av_seek_frame(context, uindex, target, flags) >= 0;
    }

    virtual void suspendOutput() = 0;
    virtual void resumeOutput() = 0;

protected:
    bool valid, pause;

    QMutex * mutex;
    AVStream * stream;
    int uindex;
    AVCodecContext * codec_context;
    AVCodec * codec;
    AVFrame * frame;

    QList<AVPacket*> packets;
};

#endif // I_MEDIA_STREAM_H
