//        QString str(av_get_pix_fmt_name(fmt));
//        QRegExp rgb_pattern("(RGB)|(BGR)");
//        qDebug() << "BBBBBBBB " << str << " " << new QString(av_get_pix_fmt_name(fmt) << 10);

#ifndef VIDEO_BUFFER_H
#define VIDEO_BUFFER_H

#include <QBitmap>
#include "media/media_player/utils/media_player_utils.h"
#include "video_settings.h"
#include "video_blending.h"

class VideoBuffer {
public:
    enum VideoBufferType {
        other,
        rgb
    };

    VideoBuffer(QImage * img, VideoSettings * settings) : av_picture(0) {
        setts = settings;
        q_image = img;
    }

    VideoBuffer(AVPicture * pict, VideoSettings * settings) : q_image(0) {
        setts = settings;
        av_picture = pict;
    }

    ~VideoBuffer() {
        if (q_image)
            delete q_image;

        if (av_picture)
            avpicture_free(av_picture);

//        av_freep(descriptor);
    }

    QImage * asQImage() { return q_image; }
    AVPicture * asAVPicture() { return av_picture; }

    int lineSize(int num) const {
        if (q_image) {
            if (num == 0)
                return q_image -> byteCount(); //q_image -> width() * q_image -> height();
            else return -1;
        } else {
            if (num < AV_NUM_DATA_POINTERS)
                return av_picture -> linesize[num];
            else return -1;
        }
    }

//    quint8* plane(int num) const {
//        if (q_image) {
//            if (num == 0)
//                return (quint8 *)q_image -> bits();
//            else return 0;
//        } else {
//            if (num < AV_NUM_DATA_POINTERS)
//                return av_picture -> data[num];
//            else return 0;
//        }
//    }

    int planeWidth(int plane) const {
        return lineSize(plane) / setts -> bytesPerPixel(plane);
    }

    int planeHeight(int plane) const {
        if (plane == 0)
            return setts -> buff_height;
        return setts -> chromaHeight(setts -> buff_height);
    }

    void blend_subrect(const AVSubtitleRect *rect) {
        if (q_image)
            VideoBlending::blend_subrect(q_image, rect);
        else
            VideoBlending::blend_subrect(av_picture, setts -> buff_width, setts -> buff_height, rect);
    }

    enum VideoBufferType type() const {
        if (q_image) {
            return rgb;
        } else return other;
    }

    inline VideoSettings * settings() const { return setts; }

private:
    VideoSettings * setts;
    VideoBufferType buff_type;
    QImage * q_image;
    AVPicture * av_picture;
};

#endif // VIDEO_BUFFER_H


//#ifndef VIDEO_BUFFER_H
//#define VIDEO_BUFFER_H

//#include <QImage>

//struct VideoBuffer {
//    VideoBuffer(void * data, QList<QSize>) {

//    }

//    ~VideoBuffer() {
//        QImage * img = asImage();
//        if (img != 0)
//            delete asImage();
//        else {
//            uint8_t * buffers = asBuffers();
//            av_freep((uint8_t *)buffers[0]);
//            av_freep((uint8_t *)buffers[1]);
//            av_freep((uint8_t *)buffers[2]);
//            av_freep(buffers);
//        }
//    }

//    QImage * asImage() { return (QImage *)image; }
//    uint8_t * asBuffers() { return (uint8_t *)image; }

//    void * image;

//};

//#endif // VIDEO_BUFFER_H
