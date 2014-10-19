#ifndef VIDEO_RESAMPLER_H
#define VIDEO_RESAMPLER_H

#include "media/media_player/utils/video_buffer.h"

class VideoResampler {
public:
    VideoResampler(enum AVPixelFormat pixel_format_in, enum AVPixelFormat pixel_format_out = AV_PIX_FMT_RGB24);
    ~VideoResampler();

    VideoBuffer * proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut);
protected:
    VideoBuffer * toQImage(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut);
    void setColorspaceDetails(int brightness, int contrast, int saturation);
    bool isPlanar(enum PixelFormat fmt);
    bool isBigEndian(enum PixelFormat fmt);
    bool isLittleEndian(enum PixelFormat fmt);
    QImage::Format toQImageFormat(PixelFormat fmt);

private:
    enum AVPixelFormat pixelFormatIn;
    enum AVPixelFormat pixelFormatOut;
    AVFrame * RGBFrame;

    SwsContext* resampleContext;
};

#endif // VIDEO_RESAMPLER_H
