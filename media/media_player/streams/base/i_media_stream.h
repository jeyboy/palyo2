#ifndef I_MEDIA_STREAM_H
#define I_MEDIA_STREAM_H

#include "media/media_player/utils/media_player_utils.h"
#include "media/media_player/utils/master_clock.h"
#include <QMutex>

#define FRAMES_LIMIT 32
#define PACKETS_LIMIT 8

class IMediaStream {

public:
    IMediaStream(AVFormatContext * context, int streamIndex) {
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

        codec_context = stream -> codec;
        codec = avcodec_find_decoder(codec_context -> codec_id);

//        //wtf
//        if (codec -> capabilities & CODEC_CAP_TRUNCATED)
//            codec_context -> flags |= CODEC_FLAG_TRUNCATED;

//        codec_context -> flags |= CODEC_FLAG_EMU_EDGE;
//        codec_context -> flags2 |= CODEC_FLAG2_FAST;
//        if(codec -> capabilities & CODEC_CAP_DR1)
//            codec_context -> flags |= CODEC_FLAG_EMU_EDGE;
        //////

        valid = codec != 0;
        if (!valid) return;

        is_lossless = codec -> capabilities & CODEC_CAP_LOSSLESS;
        is_vbr = codec -> capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE;


        AVDictionary *opts = NULL;

        if (codec_context -> codec_type == AVMEDIA_TYPE_VIDEO || codec_context -> codec_type == AVMEDIA_TYPE_AUDIO)
            av_dict_set(&opts, "refcounted_frames", "1", 0);

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

        avcodec_close(codec_context);
        av_freep(codec_context);

        delete codec;
    }

    inline int index() const { return uindex; }

    inline bool isBlocked() { return valid && packets.size() > PACKETS_LIMIT; }
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
        mutex -> unlock();
    }

protected:
    bool valid;
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
