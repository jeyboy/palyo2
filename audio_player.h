#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QApplication>
#include <QObject>
#include <QUrl>
#include <QDir>

#include "bass.h"
#include "notify_timer.h"

void __stdcall endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);

class AudioPlayer : public QObject {
    Q_OBJECT

    Q_ENUMS(MediaStateFlags)
    Q_ENUMS(MediaStatusFlags)

    int openChannel(QString path);

public:
    enum MediaStateFlags {
        StoppedState,
        PlayingState,
        PausedState
    };
    typedef QFlags<MediaStateFlags> MediaState;

    enum MediaStatusFlags {
        UnknownMediaStatus,
        NoMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        EndOfMedia,
        InvalidMedia
    };
    typedef QFlags<MediaStatusFlags> MediaStatus;

    explicit AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    int getNotifyInterval();
    void setNotifyInterval(signed int milis);

    void setMedia(QUrl mediaPath);

    MediaState state() const;

signals:
    void playbackEnded();
    void stateChanged(MediaState);
    void mediaStatusChanged(MediaStatus);

    void positionChanged(int);
    void durationChanged(int);

private slots:
    void started();
    void stoped();
    void signalUpdate();

public slots:
    void play();
    void pause();
    void resume();
    void stop();
    void endOfPlayback();

    void setPosition(int position);

private:
    QUrl mediaUri;

    int notifyInterval;

    MediaState currentState;

    unsigned long chan;
    NotifyTimer * notifyTimer;
};

#endif // AUDIO_PLAYER_H
