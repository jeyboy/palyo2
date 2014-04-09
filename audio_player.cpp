#include "audio_player.h"
#include <QDebug>

void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user) {
    BASS_ChannelRemoveSync(channel, handle);
//    BASS_ChannelStop(channel);

    AudioPlayer * player = static_cast<AudioPlayer *>(user);
    emit player -> playbackEnded();
}

AudioPlayer::AudioPlayer(QObject * parent) : QObject(parent) {
    qRegisterMetaType<AudioPlayer::MediaStatus>("MediaStatus");
    qRegisterMetaType<AudioPlayer::MediaState>("MediaState");

    // cheat for cross treadhing
    connect(this, SIGNAL(playbackEnded()), this, SLOT(endOfPlayback()));

    notifyInterval = 100;

    currentState = StoppedState;

    if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
        throw "An incorrect version of BASS.DLL was loaded";
    }

    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Init error: " << BASS_ErrorGetCode();
//        throw "Cannot initialize device";

    ///////////////////////////////////////////////
    /// load plugins
    ///////////////////////////////////////////////
        //TODO: add loading of plugins

//        #ifdef _WIN32 // Windows/CE
//            BASS_PluginLoad("bassflac.dll", 0);
//        #elif defined(__linux__) // Linux
//            BASS_PluginLoad("libbassflac.so", 0);
//        #elif defined(TARGET_OS_IPHONE) // iOS
//            extern void BASSFLACplugin;
//            BASS_PluginLoad(&BASSFLACplugin, 0);
//        #else // OSX
//            BASS_PluginLoad("libbassflac.dylib", 0);
//        #endif
    ///////////////////////////////////////////////

    notifyTimer = new NotifyTimer(this);
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    connect(notifyTimer, SIGNAL(started()), this, SLOT(started()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(stoped()));
}

AudioPlayer::~AudioPlayer() {
    BASS_PluginFree(0);
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

void AudioPlayer::setMedia(QUrl mediaPath) {
    mediaUri = mediaPath;
}


AudioPlayer::MediaState AudioPlayer::state() const {
    return currentState;
}

////////////////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////////////////

int AudioPlayer::openChannel(QString path) {
    BASS_ChannelStop(chan);
    if (!(chan = BASS_StreamCreateFile(false, path.toLatin1(), 0, 0, BASS_SAMPLE_LOOP))
        && !(chan = BASS_MusicLoad(false, path.toLatin1(), 0, 0, BASS_MUSIC_RAMP | BASS_SAMPLE_LOOP | BASS_MUSIC_STOPBACK | BASS_STREAM_PRESCAN, 1)))
            qDebug() << "Can't play file";
    return chan;
}

////////////////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////////////////

void AudioPlayer::started() {
    currentState = PlayingState;
    emit stateChanged(PlayingState);
}

void AudioPlayer::stoped() {
    currentState = StoppedState;
}

void AudioPlayer::signalUpdate() {
    emit positionChanged(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * 1000);
}

////////////////////////////////////////////////////////////////////////

void AudioPlayer::play() {
    if (currentState == PausedState) {
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
    notifyTimer -> stop();
    BASS_ChannelPause(chan);
    emit stateChanged(PausedState);
    currentState = PausedState;
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

void AudioPlayer::setPosition(int position) {
    BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, position / 1000.0), BASS_POS_BYTE);
}
