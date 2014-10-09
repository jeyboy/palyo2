#ifndef VIDEO_RESAMPLER_H
#define VIDEO_RESAMPLER_H

#include "media/media_player/utils/media_player_utils.h"
#include <QImage>

class VideoResampler {
public:
    VideoResampler(enum AVPixelFormat pixel_format_in, enum AVPixelFormat pixel_format_out = AV_PIX_FMT_RGB24);
    ~VideoResampler();

    void * proceed2(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut);
    void * proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut);
private:
    enum AVPixelFormat pixelFormatIn;
    enum AVPixelFormat pixelFormatOut;
    AVFrame * RGBFrame;

    SwsContext* resampleContext;
};

#endif // VIDEO_RESAMPLER_H
