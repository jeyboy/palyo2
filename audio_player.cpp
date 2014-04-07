#include "audio_player.h"
#include <QDebug>

void syncFunc(HSYNC handle, DWORD channel, DWORD data, void * user) {
    BASS_ChannelRemoveSync(channel, handle);
    BASS_ChannelStop(channel);
//    endOfMusic = true;
}

AudioPlayer::AudioPlayer(QObject * parent) : QThread(parent)	{
    notifyInterval = 100;

    endOfMusic = true;
    close = false;
    playing = false;

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

void AudioPlayer::run() {
    while (!close);
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
/// SLOTS
////////////////////////////////////////////////////////////////////////

void AudioPlayer::started() {
    playing = true;
}

void AudioPlayer::stoped() {
    playing = false;
}

void AudioPlayer::signalUpdate() {
    if (endOfMusic == false) {
        emit curPos(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)),
                    BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE)));
    } else {
        emit endOfPlayback();
        playing = false;
    }
}

////////////////////////////////////////////////////////////////////////

void AudioPlayer::play() {
    BASS_ChannelStop(chan);
    if (!(chan = BASS_StreamCreateFile(false, mediaUri.toLocalFile().toLatin1(), 0, 0, BASS_SAMPLE_LOOP))
        && !(chan = BASS_MusicLoad(false, mediaUri.toLocalFile().toLatin1(), 0, 0, BASS_MUSIC_RAMP | BASS_SAMPLE_LOOP | BASS_MUSIC_STOPBACK, 1)))
            qDebug() << "Can't play file";
    else {
        endOfMusic = false;
        BASS_ChannelPlay(chan, true);
        notifyTimer -> start(notifyInterval);
        playing = true;
        BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &syncFunc, 0);
    }
}

void AudioPlayer::pause() {
    BASS_ChannelPause(chan);
    notifyTimer -> stop();
    playing = false;
}

void AudioPlayer::resume() {
    if (!BASS_ChannelPlay(chan, false))
        qDebug() << "Error resuming";
    else {
        notifyTimer -> start(notifyInterval);
        playing = true;
    }
}

void AudioPlayer::stop() {
    BASS_ChannelStop(chan);
    notifyTimer -> stop();
    playing = false;
}

void AudioPlayer::changePosition(int position) {
    BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, position), BASS_POS_BYTE);
}
