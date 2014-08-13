#include "media_stream.h"

MediaStream::MediaStream(AVFormatContext * context, int streamIndex, QObject * parent, Priority priority) : Stream(parent, priority)
  , waitMillis(12)
  , state(true)
  , finishAndPause(false)
  , stream(0)
  , codec_context(0)
  , codec(0)
  , frame(0) {

    if (streamIndex < 0 || streamIndex == AVERROR_STREAM_NOT_FOUND || streamIndex == AVERROR_DECODER_NOT_FOUND) {
        state = false;
        return;
    }

    uindex = streamIndex;

    stream = context -> streams[uindex];

    if (stream == 0) {
        state = false;
        return;
    }

    codec_context = stream -> codec;
    codec = avcodec_find_decoder(codec_context -> codec_id);

    //wtf
    if (codec -> capabilities & CODEC_CAP_TRUNCATED)
        codec_context -> flags |= CODEC_FLAG_TRUNCATED;

    codec_context -> flags |= CODEC_FLAG_EMU_EDGE;
    codec_context -> flags2 |= CODEC_FLAG2_FAST;
    if(codec -> capabilities & CODEC_CAP_DR1)
        codec_context -> flags |= CODEC_FLAG_EMU_EDGE;


//    if(codec -> capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE)
//        //Audio encoder supports receiving a different number of samples in each call.

//    if(codec -> capabilities & CODEC_CAP_LOSSLESS)
//        //Codec is lossless.

    //////

    if (codec == 0) {
        state = false;
        return;
    }


    AVDictionary *opts = NULL;

    if (codec_context -> codec_type == AVMEDIA_TYPE_VIDEO || codec_context -> codec_type == AVMEDIA_TYPE_AUDIO)
        av_dict_set(&opts, "refcounted_frames", "1", 0);

    if (avcodec_open2(codec_context, codec, &opts) < 0) {
        state = false;
        return;
    }

    frame = av_frame_alloc();

    start(priority);
}

MediaStream::~MediaStream() {
    qDebug() << " ******* " << state;

    avcodec_close(codec_context);

    if (frame)
        av_frame_free(&frame);

    foreach(AVPacket * pack, packets)
        av_free_packet(pack);

// this strings throw error
//    if (stream) {
//        avcodec_close(stream -> codec);
//    }

//    delete codec;
//    delete stream;
}

void MediaStream::decode(AVPacket * newPacket) {
    mutex -> lock();
    packets.append(newPacket);
    mutex -> unlock();
}

//void MediaStream::decode(unsigned char* bytes, int size) {
//    if (size <= 0) return;

//    AVPacket * packet = new AVPacket();
//    av_init_packet(packet);
//    packet -> size = size;
//    packet -> data = bytes;
//    packets.append(packet);
//}

void MediaStream::pauseOnComplete() {
    finishAndPause = true;
}

void MediaStream::resume() {
    finishAndPause = false;
    Stream::resume();
}
