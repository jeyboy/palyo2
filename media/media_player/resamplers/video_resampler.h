#ifndef VIDEO_RESAMPLER_H
#define VIDEO_RESAMPLER_H

#include "media/media_player/streams/frames/video/video_buffer.h"
#include "media/media_player/utils/video_types.h"

class VideoResampler {
public:
    VideoResampler(enum AVPixelFormat pixel_format_in, enum AVPixelFormat pixel_format_out = AV_PIX_FMT_RGB24);
    ~VideoResampler();

    VideoBuffer * proceed(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut);
    inline bool isGLShaderCompatible() { return compatible; }
protected:
    VideoBuffer * toQImage(AVFrame * frame, int widthIn, int heightIn, int widthOut, int heightOut);
    void setColorspaceDetails(int brightness, int contrast, int saturation);
    void initSettings(AVFrame * frame, enum AVPixelFormat pixelFormatOut, int w, int h);

private:
    enum AVPixelFormat pixelFormatIn;
    enum AVPixelFormat pixelFormatOut;
    AVFrame * RGBFrame;
    QImage::Format img_format;
    bool compatible;
    VideoSettings * settings;

    SwsContext* resampleContext;
};

#endif // VIDEO_RESAMPLER_H
