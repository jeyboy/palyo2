#include "media_player.h"

MediaPlayer::MediaPlayer(QWidget * parent) : QWidget(parent) {
    avcodec_register_all();
}
