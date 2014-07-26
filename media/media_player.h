#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QWidget>
#include <QUrl>

extern "C" {
    #include "media_player_libs.h"
}

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

class MediaPlayer : public QWidget {
    Q_OBJECT
public:

    MediaPlayer(QWidget * parent = 0);

protected:
    bool openObject(QUrl url);
    void closeObject();

    bool nextFrame();

private:

//    /* options specified by the user */
//    AVInputFormat *file_iformat;
//    const char *input_filename;
////    const char * window_title;
//    int fs_screen_width;
//    int fs_screen_height;
//    int default_width  = 640;
//    int default_height = 480;
//    int screen_width  = 0;
//    int screen_height = 0;
//    int audio_disable;
//    int video_disable;
//    int subtitle_disable;
//    int wanted_stream[AVMEDIA_TYPE_NB];
//    int seek_by_bytes = -1;
//    int display_disable;
//    int show_status = 1;
//    int av_sync_type = AV_SYNC_AUDIO_MASTER;
//    int64_t start_time = AV_NOPTS_VALUE;
//    int64_t duration = AV_NOPTS_VALUE;
//    int workaround_bugs = 1;
//    int fast = 0;
//    int genpts = 0;
//    int lowres = 0;
//    int decoder_reorder_pts = -1;
//    int autoexit;
//    int exit_on_keydown;
//    int exit_on_mousedown;
//    int loop = 1;
//    int framedrop = -1;
//    int infinite_buffer = -1;
//    enum ShowMode show_mode = SHOW_MODE_NONE;
//    const char *audio_codec_name;
//    const char *subtitle_codec_name;
//    const char *video_codec_name;
//    double rdftspeed = 0.02;
//    int64_t cursor_last_shown;
//    int cursor_hidden = 0;
//    #if CONFIG_AVFILTER
//    const char **vfilters_list = NULL;
//    int nb_vfilters = 0;
//    char *afilters = NULL;
//    #endif
//    int autorotate = 1;

//    int is_full_screen;
//    int64_t audio_callback_time;

//    AVPacket flush_pkt;

    AVFormatContext * currObj;
    AVPacket * currFrame;
    QWidget * screen;
    bool isRemote;
};

#endif // MEDIA_PLAYER_H
