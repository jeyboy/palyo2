#ifndef MEDIA_PLAYER_UTILS_H
#define MEDIA_PLAYER_UTILS_H

//#define INBUF_SIZE 4096
//#define AUDIO_INBUF_SIZE 20480
//#define AUDIO_REFILL_THRESH 4096

#ifndef INT64_MAX
    #define INT64_MAX 9223372036854775807LL
#endif

#ifndef INT64_MIN
    #define INT64_MIN -INT64_MAX
#endif

#include <QDebug>

#define __STDC_CONSTANT_MACROS
extern "C" {
    #include "media_player_libs.h"
}

class MediaPlayerUtils {
public:
    static AVSampleFormat toSampleFormat(int fmt) {
        switch(fmt) {
            case 8: return AV_SAMPLE_FMT_U8;
            case 16: return AV_SAMPLE_FMT_S16;
            case 32: return AV_SAMPLE_FMT_S32;
            default: return AV_SAMPLE_FMT_S16;
        }
    }

    //    static AVSampleFormat compatibleCodecType(AVCodec *codec) {
    //        if (!codec -> sample_fmts)
    //            return AV_SAMPLE_FMT_S16;

    //        const enum AVSampleFormat *sample_fmts;
    //        AVSampleFormat best_format = AV_SAMPLE_FMT_U8;

    //        sample_fmts = codec -> sample_fmts;
    //        while (*sample_fmts) {
    //            if (*sample_fmts == AV_SAMPLE_FMT_S16 || *sample_fmts == AV_SAMPLE_FMT_S32
    //                    || *sample_fmts == AV_SAMPLE_FMT_FLT || *sample_fmts == AV_SAMPLE_FMT_DBL)
    //                best_format = *sample_fmts;
    //            sample_fmts++;
    //        }

    //        return best_format;
    //    }

    /* just pick the highest supported samplerate */
    static int selectSampleRate(AVCodec *codec) {
        if (!codec -> supported_samplerates)
            return 44100;

        int best_samplerate = 0;
        const int *p = codec -> supported_samplerates;

        while (*p) {
            best_samplerate = FFMAX(*p, best_samplerate);
            p++;
        }

        return best_samplerate;
    }

    /* select layout with the highest channel count */
    static int selectChannelLayout(AVCodec *codec) {
        if (!codec -> channel_layouts)
            return AV_CH_LAYOUT_STEREO;

        const uint64_t *p;
        uint64_t best_ch_layout = 0;
        int best_nb_channels = 0;

        p = codec -> channel_layouts;
        while (*p) {
            int nb_channels = av_get_channel_layout_nb_channels(*p);

            if (nb_channels > best_nb_channels) {
                best_ch_layout = *p;
                best_nb_channels = nb_channels;
            }
            p++;
        }

        return best_ch_layout;
    }

    static int checkChannelLayout(int channel_layout, int channels_count) {
        if (channel_layout && av_get_channel_layout_nb_channels(channel_layout) == channels_count)
            return channel_layout;
        else
            return av_get_default_channel_layout(channels_count);
//            return 0;
    }











//    // Setup QAudioInput and QAudioOutput, not shown for brevity. See the Spectrum example for details
//    QAudioInput input;
//    QAudioOutput output;
//    QIODevice *intermediateDevice; // This very temporarily stores recorded audio and then transfers it to the circular buffer

//    // Initialize the circular buffer, also shown in more detail in the Spectrum example
//    QByteArray byteArray[NUM_CIRCLE_BUFFER_ELEMENTS];  // Note that the spectrum example names this variable buffer despite it not being a QBuffer
//    qint64 byteArrayUsed[NUM_CIRCLE_BUFFER_ELEMENTS]; // This tracks how much of each buffer has been used

//    // Start recording audio
//    intermediateDevice = input->start();
//    connect(intermediateDevice, SIGNAL(readyRead()), this, SLOT(captureData())); // captureData is a function you define, and example is below

//    // Example captureData
//    void captureData(qint64 bytesToCapture)
//    {
//      qint64 bytesAvailable = byteArray[current].size() - byteArrayUsed[current];
//      qint64 bytesToRead = qMin(bytesToCapture, bytesAvailable);

//      // Transfer the audio from the intermediateDevice to the buffer
//      bytesActuallyRead = intermediateDevice->read(byteArray[current].data() + byteArrayUsed[current], bytesToRead);

//      byteArrayUsed[current] += bytesActuallyRead;

//      // Note that this is the simplified version, you would want to have the transferring in a loop and do this again in the next byteArray if bytesActuallyRead < bytesToRead
//    }











//    bool startRecording(QUrl url, AVCodecID newFormat = AV_CODEC_ID_MP2);
//    void stopRecording();




    //bool MediaPlayer::startRecording(QUrl url, AVCodecID newFormat = AV_CODEC_ID_MP2) {
    //    AVCodec * codec;
    //    AVFrame * frame;
    //    AVPacket pkt;
    //    int ret, got_output;

    //    outputFile = new QFile(url.toLocalFile());
    //    outputFile -> open(QIODevice::WriteOnly | QIODevice::Truncate);

    //    codec = avcodec_find_encoder(newFormat);
    //    if (!codec) { return false; } // did not find compatible codec

    //    AVCodecContext * context = avcodec_alloc_context3(codec);
    //    if (!context) { return false; } // did not create context

    //    /* put sample parameters */
    //    context -> bit_rate = 64000;
    //    /* check that the encoder supports s16 pcm input */
    //    context -> sample_fmt = AV_SAMPLE_FMT_S16;

    //    if (!check_sample_fmt(codec, context -> sample_fmt)) { return false; } //"Encoder does not support sample format " + av_get_sample_fmt_name(context -> sample_fmt));

    //    /* select other audio parameters supported by the encoder */
    //    context -> sample_rate = selectSampleRate(codec);
    //    context -> channel_layout = selectChannelLayout(codec);
    //    context -> channels = av_get_channel_layout_nb_channels(context -> channel_layout);

    //    /* open it */
    //    if (avcodec_open2(c, codec, NULL) < 0) { return false; } //        "Could not open codec";

    //    /* frame containing input raw audio */
    //    frame = av_frame_alloc();
    //    if (!frame) { return false; } //"Could not allocate audio frame"

    //    frame -> nb_samples = context -> frame_size;
    //    frame -> format = context -> sample_fmt;
    //    frame -> channel_layout = context -> channel_layout;

    //    /* the codec gives us the frame size, in samples,
    //    * we calculate the size of the samples buffer in bytes */
    //    int buffer_size = av_samples_get_buffer_size(NULL, context -> channels, context -> frame_size, context -> sample_fmt, 0);
    //    if (buffer_size < 0) { return false; } //        "Could not get sample buffer size";

    //    samples = av_malloc(buffer_size);
    //    if (!samples) { return false; } // "Could not allocate %d bytes for samples buffer", buffer_size);

    //    /* setup the data pointers in the AVFrame */
    //    ret = avcodec_fill_audio_frame(frame, context -> channels, context -> sample_fmt, (const uint8_t*)samples, buffer_size, 0);
    //    if (ret < 0) { return false; } // "Could not setup audio frame"


    //    QAudioFormat format;
    //    format.setSampleRate(context -> sample_rate);
    //    format.setSampleSize(16);
    //    format.setCodec("audio/pcm");
    //    format.setByteOrder(QAudioFormat::LittleEndian);
    //    format.setSampleType(QAudioFormat::UnSignedInt);


    //    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    //    if (!info.isFormatSupported(format)) {
    //        qWarning() << "default format not supported try to use nearest";
    //        format = info.nearestFormat(format);
    //    }

    //    audioInput = new QAudioInput(format, this);

    //    QBuffer * buffer = new QBuffer();
    //    buffer -> open(QIODevice::ReadWrite);
    ////    bytesWritten(qint64 bytes); connection need

    //    audioInput -> start(buffer);

    //    while(true) {
    //       av_init_packet(&pkt);
    //       pkt.data = NULL; // packet data will be allocated by the encoder
    //       pkt.size = 0;
    //       memcpy(samples, buffer -> data(), buffer -> data().count());

    //       ret = avcodec_encode_audio2(context, &pkt, frame, &got_output);
    //       if (ret < 0) {
    //           fprintf(stderr, "Error encoding audio frame\n");
    //           exit(1);
    //       }

    //       if (got_output) {
    //           outputFile -> write(pkt.data, pkt.size);
    ////           fwrite(pkt.data, 1, pkt.size, f);
    //           av_free_packet(&pkt);
    //       }
    //    }

    //    outputFile -> close();
    //    av_freep(&samples);
    //    av_frame_free(&frame);
    //    avcodec_close(c);
    //    av_free(c);
    //}

//    void MediaPlayer::stopRecording() {
//        if (audioInput)
//            audioInput -> stop();
//        delete audioInput;
//    }








///* check that a given sample format is supported by the encoder */
//static int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
//{
//    const enum AVSampleFormat *p = codec->sample_fmts;

//    while (*p != AV_SAMPLE_FMT_NONE) {
//        if (*p == sample_fmt)
//            return 1;
//        p++;
//    }
//    return 0;
//}

///* just pick the highest supported samplerate */
//static int select_sample_rate(AVCodec *codec)
//{
//    const int *p;
//    int best_samplerate = 0;

//    if (!codec->supported_samplerates)
//        return 44100;

//    p = codec->supported_samplerates;
//    while (*p) {
//        best_samplerate = FFMAX(*p, best_samplerate);
//        p++;
//    }
//    return best_samplerate;
//}

///* select layout with the highest channel count */
//static int select_channel_layout(AVCodec *codec)
//{
//    const uint64_t *p;
//    uint64_t best_ch_layout = 0;
//    int best_nb_channels   = 0;

//    if (!codec->channel_layouts)
//        return AV_CH_LAYOUT_STEREO;

//    p = codec->channel_layouts;
//    while (*p) {
//        int nb_channels = av_get_channel_layout_nb_channels(*p);

//        if (nb_channels > best_nb_channels) {
//            best_ch_layout    = *p;
//            best_nb_channels = nb_channels;
//        }
//        p++;
//    }
//    return best_ch_layout;
//}

///*
// * Audio encoding example
// */
//static void audio_encode_example(const char *filename)
//{
//    AVCodec *codec;
//    AVCodecContext *c= NULL;
//    AVFrame *frame;
//    AVPacket pkt;
//    int i, j, k, ret, got_output;
//    int buffer_size;
//    FILE *f;
//    uint16_t *samples;
//    float t, tincr;

//    printf("Encode audio file %s\n", filename);

//    /* find the MP2 encoder */
//    codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
//    if (!codec) {
//        fprintf(stderr, "Codec not found\n");
//        exit(1);
//    }

//    c = avcodec_alloc_context3(codec);
//    if (!c) {
//        fprintf(stderr, "Could not allocate audio codec context\n");
//        exit(1);
//    }

//    /* put sample parameters */
//    c->bit_rate = 64000;

//    /* check that the encoder supports s16 pcm input */
//    c->sample_fmt = AV_SAMPLE_FMT_S16;
//    if (!check_sample_fmt(codec, c->sample_fmt)) {
//        fprintf(stderr, "Encoder does not support sample format %s",
//                av_get_sample_fmt_name(c->sample_fmt));
//        exit(1);
//    }

//    /* select other audio parameters supported by the encoder */
//    c->sample_rate    = select_sample_rate(codec);
//    c->channel_layout = select_channel_layout(codec);
//    c->channels       = av_get_channel_layout_nb_channels(c->channel_layout);

//    /* open it */
//    if (avcodec_open2(c, codec, NULL) < 0) {
//        fprintf(stderr, "Could not open codec\n");
//        exit(1);
//    }

//    f = fopen(filename, "wb");
//    if (!f) {
//        fprintf(stderr, "Could not open %s\n", filename);
//        exit(1);
//    }

//    /* frame containing input raw audio */
//    frame = av_frame_alloc();
//    if (!frame) {
//        fprintf(stderr, "Could not allocate audio frame\n");
//        exit(1);
//    }

//    frame->nb_samples     = c->frame_size;
//    frame->format         = c->sample_fmt;
//    frame->channel_layout = c->channel_layout;

//    /* the codec gives us the frame size, in samples,
//     * we calculate the size of the samples buffer in bytes */
//    buffer_size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size,
//                                             c->sample_fmt, 0);
//    if (buffer_size < 0) {
//        fprintf(stderr, "Could not get sample buffer size\n");
//        exit(1);
//    }
//    samples = av_malloc(buffer_size);
//    if (!samples) {
//        fprintf(stderr, "Could not allocate %d bytes for samples buffer\n",
//                buffer_size);
//        exit(1);
//    }
//    /* setup the data pointers in the AVFrame */
//    ret = avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt,
//                                   (const uint8_t*)samples, buffer_size, 0);
//    if (ret < 0) {
//        fprintf(stderr, "Could not setup audio frame\n");
//        exit(1);
//    }

//    /* encode a single tone sound */
//    t = 0;
//    tincr = 2 * M_PI * 440.0 / c->sample_rate;
//    for (i = 0; i < 200; i++) {
//        av_init_packet(&pkt);
//        pkt.data = NULL; // packet data will be allocated by the encoder
//        pkt.size = 0;

//        for (j = 0; j < c->frame_size; j++) {
//            samples[2*j] = (int)(sin(t) * 10000);

//            for (k = 1; k < c->channels; k++)
//                samples[2*j + k] = samples[2*j];
//            t += tincr;
//        }
//        /* encode the samples */
//        ret = avcodec_encode_audio2(c, &pkt, frame, &got_output);
//        if (ret < 0) {
//            fprintf(stderr, "Error encoding audio frame\n");
//            exit(1);
//        }
//        if (got_output) {
//            fwrite(pkt.data, 1, pkt.size, f);
//            av_free_packet(&pkt);
//        }
//    }

//    /* get the delayed frames */
//    for (got_output = 1; got_output; i++) {
//        ret = avcodec_encode_audio2(c, &pkt, NULL, &got_output);
//        if (ret < 0) {
//            fprintf(stderr, "Error encoding frame\n");
//            exit(1);
//        }

//        if (got_output) {
//            fwrite(pkt.data, 1, pkt.size, f);
//            av_free_packet(&pkt);
//        }
//    }
//    fclose(f);

//    av_freep(&samples);
//    av_frame_free(&frame);
//    avcodec_close(c);
//    av_free(c);
//}

///*
// * Audio decoding.
// */
//static void audio_decode_example(const char *outfilename, const char *filename)
//{
//    AVCodec *codec;
//    AVCodecContext *c= NULL;
//    int len;
//    FILE *f, *outfile;
//    uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
//    AVPacket avpkt;
//    AVFrame *decoded_frame = NULL;

//    av_init_packet(&avpkt);

//    printf("Decode audio file %s to %s\n", filename, outfilename);

//    /* find the mpeg audio decoder */
//    codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
//    if (!codec) {
//        fprintf(stderr, "Codec not found\n");
//        exit(1);
//    }

//    c = avcodec_alloc_context3(codec);
//    if (!c) {
//        fprintf(stderr, "Could not allocate audio codec context\n");
//        exit(1);
//    }

//    /* open it */
//    if (avcodec_open2(c, codec, NULL) < 0) {
//        fprintf(stderr, "Could not open codec\n");
//        exit(1);
//    }

//    f = fopen(filename, "rb");
//    if (!f) {
//        fprintf(stderr, "Could not open %s\n", filename);
//        exit(1);
//    }
//    outfile = fopen(outfilename, "wb");
//    if (!outfile) {
//        av_free(c);
//        exit(1);
//    }

//    /* decode until eof */
//    avpkt.data = inbuf;
//    avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

//    while (avpkt.size > 0) {
//        int got_frame = 0;

//        if (!decoded_frame) {
//            if (!(decoded_frame = av_frame_alloc())) {
//                fprintf(stderr, "Could not allocate audio frame\n");
//                exit(1);
//            }
//        }

//        len = avcodec_decode_audio4(c, decoded_frame, &got_frame, &avpkt);
//        if (len < 0) {
//            fprintf(stderr, "Error while decoding\n");
//            exit(1);
//        }
//        if (got_frame) {
//            /* if a frame has been decoded, output it */
//            int data_size = av_samples_get_buffer_size(NULL, c->channels,
//                                                       decoded_frame->nb_samples,
//                                                       c->sample_fmt, 1);
//            if (data_size < 0) {
//                /* This should not occur, checking just for paranoia */
//                fprintf(stderr, "Failed to calculate data size\n");
//                exit(1);
//            }
//            fwrite(decoded_frame->data[0], 1, data_size, outfile);
//        }
//        avpkt.size -= len;
//        avpkt.data += len;
//        avpkt.dts =
//        avpkt.pts = AV_NOPTS_VALUE;
//        if (avpkt.size < AUDIO_REFILL_THRESH) {
//            /* Refill the input buffer, to avoid trying to decode
//             * incomplete frames. Instead of this, one could also use
//             * a parser, or use a proper container format through
//             * libavformat. */
//            memmove(inbuf, avpkt.data, avpkt.size);
//            avpkt.data = inbuf;
//            len = fread(avpkt.data + avpkt.size, 1,
//                        AUDIO_INBUF_SIZE - avpkt.size, f);
//            if (len > 0)
//                avpkt.size += len;
//        }
//    }

//    fclose(outfile);
//    fclose(f);

//    avcodec_close(c);
//    av_free(c);
//    av_frame_free(&decoded_frame);
//}

///*
// * Video encoding example
// */
//static void video_encode_example(const char *filename, int codec_id)
//{
//    AVCodec *codec;
//    AVCodecContext *c= NULL;
//    int i, ret, x, y, got_output;
//    FILE *f;
//    AVFrame *frame;
//    AVPacket pkt;
//    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

//    printf("Encode video file %s\n", filename);

//    /* find the mpeg1 video encoder */
//    codec = avcodec_find_encoder(codec_id);
//    if (!codec) {
//        fprintf(stderr, "Codec not found\n");
//        exit(1);
//    }

//    c = avcodec_alloc_context3(codec);
//    if (!c) {
//        fprintf(stderr, "Could not allocate video codec context\n");
//        exit(1);
//    }

//    /* put sample parameters */
//    c->bit_rate = 400000;
//    /* resolution must be a multiple of two */
//    c->width = 352;
//    c->height = 288;
//    /* frames per second */
//    c->time_base = (AVRational){1,25};
//    /* emit one intra frame every ten frames
//     * check frame pict_type before passing frame
//     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
//     * then gop_size is ignored and the output of encoder
//     * will always be I frame irrespective to gop_size
//     */
//    c->gop_size = 10;
//    c->max_b_frames = 1;
//    c->pix_fmt = AV_PIX_FMT_YUV420P;

//    if (codec_id == AV_CODEC_ID_H264)
//        av_opt_set(c->priv_data, "preset", "slow", 0);

//    /* open it */
//    if (avcodec_open2(c, codec, NULL) < 0) {
//        fprintf(stderr, "Could not open codec\n");
//        exit(1);
//    }

//    f = fopen(filename, "wb");
//    if (!f) {
//        fprintf(stderr, "Could not open %s\n", filename);
//        exit(1);
//    }

//    frame = av_frame_alloc();
//    if (!frame) {
//        fprintf(stderr, "Could not allocate video frame\n");
//        exit(1);
//    }
//    frame->format = c->pix_fmt;
//    frame->width  = c->width;
//    frame->height = c->height;

//    /* the image can be allocated by any means and av_image_alloc() is
//     * just the most convenient way if av_malloc() is to be used */
//    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
//                         c->pix_fmt, 32);
//    if (ret < 0) {
//        fprintf(stderr, "Could not allocate raw picture buffer\n");
//        exit(1);
//    }

//    /* encode 1 second of video */
//    for (i = 0; i < 25; i++) {
//        av_init_packet(&pkt);
//        pkt.data = NULL;    // packet data will be allocated by the encoder
//        pkt.size = 0;

//        fflush(stdout);
//        /* prepare a dummy image */
//        /* Y */
//        for (y = 0; y < c->height; y++) {
//            for (x = 0; x < c->width; x++) {
//                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
//            }
//        }

//        /* Cb and Cr */
//        for (y = 0; y < c->height/2; y++) {
//            for (x = 0; x < c->width/2; x++) {
//                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
//                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
//            }
//        }

//        frame->pts = i;

//        /* encode the image */
//        ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
//        if (ret < 0) {
//            fprintf(stderr, "Error encoding frame\n");
//            exit(1);
//        }

//        if (got_output) {
//            printf("Write frame %3d (size=%5d)\n", i, pkt.size);
//            fwrite(pkt.data, 1, pkt.size, f);
//            av_free_packet(&pkt);
//        }
//    }

//    /* get the delayed frames */
//    for (got_output = 1; got_output; i++) {
//        fflush(stdout);

//        ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
//        if (ret < 0) {
//            fprintf(stderr, "Error encoding frame\n");
//            exit(1);
//        }

//        if (got_output) {
//            printf("Write frame %3d (size=%5d)\n", i, pkt.size);
//            fwrite(pkt.data, 1, pkt.size, f);
//            av_free_packet(&pkt);
//        }
//    }

//    /* add sequence end code to have a real mpeg file */
//    fwrite(endcode, 1, sizeof(endcode), f);
//    fclose(f);

//    avcodec_close(c);
//    av_free(c);
//    av_freep(&frame->data[0]);
//    av_frame_free(&frame);
//    printf("\n");
//}

///*
// * Video decoding example
// */

//static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
//                     char *filename)
//{
//    FILE *f;
//    int i;

//    f = fopen(filename,"w");
//    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
//    for (i = 0; i < ysize; i++)
//        fwrite(buf + i * wrap, 1, xsize, f);
//    fclose(f);
//}

//static int decode_write_frame(const char *outfilename, AVCodecContext *avctx,
//                              AVFrame *frame, int *frame_count, AVPacket *pkt, int last)
//{
//    int len, got_frame;
//    char buf[1024];

//    len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);
//    if (len < 0) {
//        fprintf(stderr, "Error while decoding frame %d\n", *frame_count);
//        return len;
//    }
//    if (got_frame) {
//        printf("Saving %sframe %3d\n", last ? "last " : "", *frame_count);
//        fflush(stdout);

//        /* the picture is allocated by the decoder, no need to free it */
//        snprintf(buf, sizeof(buf), outfilename, *frame_count);
//        pgm_save(frame->data[0], frame->linesize[0],
//                 avctx->width, avctx->height, buf);
//        (*frame_count)++;
//    }
//    if (pkt->data) {
//        pkt->size -= len;
//        pkt->data += len;
//    }
//    return 0;
//}

//static void video_decode_example(const char *outfilename, const char *filename)
//{
//    AVCodec *codec;
//    AVCodecContext *c= NULL;
//    int frame_count;
//    FILE *f;
//    AVFrame *frame;
//    uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
//    AVPacket avpkt;

//    av_init_packet(&avpkt);

//    /* set end of buffer to 0 (this ensures that no overreading happens for damaged mpeg streams) */
//    memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);

//    printf("Decode video file %s to %s\n", filename, outfilename);

//    /* find the mpeg1 video decoder */
//    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
//    if (!codec) {
//        fprintf(stderr, "Codec not found\n");
//        exit(1);
//    }

//    c = avcodec_alloc_context3(codec);
//    if (!c) {
//        fprintf(stderr, "Could not allocate video codec context\n");
//        exit(1);
//    }

//    if(codec->capabilities&CODEC_CAP_TRUNCATED)
//        c->flags|= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */

//    /* For some codecs, such as msmpeg4 and mpeg4, width and height
//       MUST be initialized there because this information is not
//       available in the bitstream. */

//    /* open it */
//    if (avcodec_open2(c, codec, NULL) < 0) {
//        fprintf(stderr, "Could not open codec\n");
//        exit(1);
//    }

//    f = fopen(filename, "rb");
//    if (!f) {
//        fprintf(stderr, "Could not open %s\n", filename);
//        exit(1);
//    }

//    frame = av_frame_alloc();
//    if (!frame) {
//        fprintf(stderr, "Could not allocate video frame\n");
//        exit(1);
//    }

//    frame_count = 0;
//    for (;;) {
//        avpkt.size = fread(inbuf, 1, INBUF_SIZE, f);
//        if (avpkt.size == 0)
//            break;

//        /* NOTE1: some codecs are stream based (mpegvideo, mpegaudio)
//           and this is the only method to use them because you cannot
//           know the compressed data size before analysing it.

//           BUT some other codecs (msmpeg4, mpeg4) are inherently frame
//           based, so you must call them with all the data for one
//           frame exactly. You must also initialize 'width' and
//           'height' before initializing them. */

//        /* NOTE2: some codecs allow the raw parameters (frame size,
//           sample rate) to be changed at any frame. We handle this, so
//           you should also take care of it */

//        /* here, we use a stream based decoder (mpeg1video), so we
//           feed decoder and see if it could decode a frame */
//        avpkt.data = inbuf;
//        while (avpkt.size > 0)
//            if (decode_write_frame(outfilename, c, frame, &frame_count, &avpkt, 0) < 0)
//                exit(1);
//    }

//    /* some codecs, such as MPEG, transmit the I and P frame with a
//       latency of one frame. You must do the following to have a
//       chance to get the last frame of the video */
//    avpkt.data = NULL;
//    avpkt.size = 0;
//    decode_write_frame(outfilename, c, frame, &frame_count, &avpkt, 1);

//    fclose(f);

//    avcodec_close(c);
//    av_free(c);
//    av_frame_free(&frame);
//    printf("\n");
//}
};

#endif // MEDIA_PLAYER_UTILS_H