#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QThread>
#include <QUrl>

#include "bass.h"
#include "notify_timer.h"

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void * user);

class AudioPlayer : public QThread {
    Q_OBJECT

    void run();
public:
    enum State {
        StoppedState,
        PlayingState,
        PausedState
    };

    enum MediaStatus {
        UnknownMediaStatus,
        NoMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        EndOfMedia,
        InvalidMedia
    };

    explicit AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    int getNotifyInterval();
    void setNotifyInterval(signed int milis);

    void setMedia(QUrl);

signals:
    void stateChanged(AudioPlayer::State);
    void mediaStatusChanged(AudioPlayer::MediaStatus);


    void endOfPlayback();
    void curPos(double Position, double Total);

private slots:
    void started();
    void stoped();
    void signalUpdate();

public slots:
    void play();
    void pause();
    void resume();
    void stop();

    void changePosition(int position);

private:
    QUrl mediaUri;

    int notifyInterval;

    bool close;

    bool playing;
    bool endOfMusic;

    unsigned long chan;
    NotifyTimer * notifyTimer;
};

#endif // AUDIO_PLAYER_H
