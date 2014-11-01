#ifndef VIDEO_BUFFER_PROXY_H
#define VIDEO_BUFFER_PROXY_H

#include "media/media_player/utils/media_player_utils.h"

struct VideoBufferProxy {
    VideoBufferProxy(AVFrame * frame) : frame_obj(0) {
        pict = new AVPicture();

        if (avpicture_alloc(pict, (AVPixelFormat)frame -> format, frame -> width, frame -> height) == 0) {
            qDebug() << "lalka";
            frame_obj = frame;

            memcpy(origin_data, frame -> data, AV_NUM_DATA_POINTERS);
            memcpy(origin_linesize, frame -> linesize, AV_NUM_DATA_POINTERS);

            memcpy(frame -> data, pict -> data, AV_NUM_DATA_POINTERS);
            memcpy(frame -> linesize, pict -> linesize, AV_NUM_DATA_POINTERS);
        }
    }

    ~VideoBufferProxy() {
        if (frame_obj) restore();
//        avpicture_free(pict); // free later
    }

    void restore() {
        memcpy(frame_obj -> data, origin_data, AV_NUM_DATA_POINTERS);
        memcpy(frame_obj -> linesize, origin_linesize, AV_NUM_DATA_POINTERS);
    }

    AVFrame * frame_obj;
    AVPicture * pict;
    uint8_t * origin_data[AV_NUM_DATA_POINTERS];
    int origin_linesize[AV_NUM_DATA_POINTERS];
};

#endif // VIDEO_BUFFER_PROXY_H
