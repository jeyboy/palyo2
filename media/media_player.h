#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QWidget>

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
};

#endif // MEDIA_PLAYER_H
