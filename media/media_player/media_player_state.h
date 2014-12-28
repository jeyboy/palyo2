#ifndef MEDIA_PLAYER_STATE
#define MEDIA_PLAYER_STATE

class MediaPlayerState {
public:
    enum Type {
        Played = -1,
        Stoped = 0,
        Paused = 1,
        Error = 2,
    };
};

#endif // MEDIA_PLAYER_STATE

