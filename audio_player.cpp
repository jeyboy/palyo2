#include "audio_player.h"
#include <QDebug>

void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user) {
    BASS_ChannelRemoveSync(channel, handle);
//    BASS_ChannelStop(channel);

    AudioPlayer * player = static_cast<AudioPlayer *>(user);
    player -> endOfPlayback();
}

AudioPlayer::AudioPlayer(QObject * parent) : QThread(parent)	{
    notifyInterval = 100;

    close = false;
    playing = false;
    paused = false;

    if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
        throw "An incorrect version of BASS.DLL was loaded";
    }

    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Init error: " << BASS_ErrorGetCode();
//        throw "Cannot initialize device";

    notifyTimer = new NotifyTimer(this);
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    connect(notifyTimer, SIGNAL(started()), this, SLOT(started()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(stoped()));
    run();
}

AudioPlayer::~AudioPlayer() {
    close = true;
    notifyTimer -> stop();
    delete notifyTimer;
}

int AudioPlayer::getNotifyInterval() {
    return notifyInterval;
}
void AudioPlayer::setNotifyInterval(signed int milis) {
    notifyInterval = milis;
    if (notifyTimer -> isActive())
        notifyTimer -> setInterval(notifyInterval);
}


////////////////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////////////////

void AudioPlayer::run() {
    while (!close);
}

int AudioPlayer::openChannel(QString path) {
    BASS_ChannelStop(chan);
    if (!(chan = BASS_StreamCreateFile(false, path.toLatin1(), 0, 0, BASS_SAMPLE_LOOP))
        && !(chan = BASS_MusicLoad(false, path.toLatin1(), 0, 0, BASS_MUSIC_RAMP | BASS_SAMPLE_LOOP | BASS_MUSIC_STOPBACK, 1)))
            qDebug() << "Can't play file";
    return chan;
}

////////////////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////////////////

void AudioPlayer::started() {
    paused = !(playing = true);
    emit stateChanged(PlayingState);
}

void AudioPlayer::stoped() {
    playing = false;
    paused = false;
    emit stateChanged(StoppedState);
}

void AudioPlayer::signalUpdate() {
    emit positionChanged(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * 1000);
}

////////////////////////////////////////////////////////////////////////

void AudioPlayer::play() {
    if (paused) {
        resume();
    } else {
        if (mediaUri.isEmpty()) {
            emit mediaStatusChanged(NoMedia);
        } else {
            if (openChannel(mediaUri.toLocalFile())) {
                durationChanged(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE)) * 1000);
                BASS_ChannelPlay(chan, true);
                notifyTimer -> start(notifyInterval);
                BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &endTrackSync, this);
            } else {
                qDebug() << "Can't play file";
            }
        }
    }
}

void AudioPlayer::pause() {
    BASS_ChannelPause(chan);
    notifyTimer -> stop();
    emit stateChanged(PausedState);
    paused = true;
}

void AudioPlayer::resume() {
    if (!BASS_ChannelPlay(chan, false)) {
        emit mediaStatusChanged(StalledMedia);
        qDebug() << "Error resuming";
    } else {
        notifyTimer -> start(notifyInterval);
    }
}

void AudioPlayer::stop() {
    BASS_ChannelStop(chan);
    notifyTimer -> stop();
    emit stateChanged(StoppedState);
}

void AudioPlayer::endOfPlayback() {
    emit mediaStatusChanged(EndOfMedia);
    stop();
}

void AudioPlayer::changePosition(int position) {
    BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, position), BASS_POS_BYTE);
}
