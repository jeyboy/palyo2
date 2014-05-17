#include "player.h"
#include <QDebug>


//"/home/user/dst.txt"


Player * Player::self = 0;

Player * Player::instance() {
    if(!self)
        self = new Player();
    return self;
}

ModelItem * Player::playedItem() const {
    return played;
}
View * Player::currentPlaylist() const {
    return playlist;
}
View * Player::currentActivePlaylist() const {
    return playlist;
}


void Player::setActivePlaylist(View * newActivePlaylist) {
    activePlaylist = newActivePlaylist;
}

void Player::setPlayButton(QAction * playAction) {
    playButton = playAction;
    playButton -> setVisible(true);
//    connect((QObject *)playAction, SIGNAL(triggered(bool)), instance(), SLOT(play()));
    connect((QObject *)playAction, SIGNAL(triggered(bool)), this, SLOT(start()));
}
void Player::setPauseButton(QAction * pauseAction) {
    pauseButton = pauseAction;
    pauseButton -> setVisible(false);
    connect((QObject *)pauseAction, SIGNAL(triggered(bool)), this, SLOT(pause()));
}
void Player::setStopButton(QAction * stopAction) {
    stopButton = stopAction;
    stopButton -> setVisible(false);
    connect((QObject *)stopAction, SIGNAL(triggered(bool)), this, SLOT(stop()));
}

void Player::setLikeButton(QAction * likeAction) {
    likeButton = likeAction;
    likeButton -> setVisible(false);
    connect((QObject *)likeAction, SIGNAL(triggered(bool)), this, SLOT(like()));
}

void Player::setMuteButton(QAction * muteAction) {
    muteButton = muteAction;
    connect((QObject *)muteButton, SIGNAL(triggered(bool)), this, SLOT(mute()));
}

void Player::setPlaylist(View * newPlaylist) {
   stop();
   updatePlaylist(newPlaylist);
   updateItem(0);
}

void Player::removePlaylist() {
    switch(state()) {
        case PausedState:
        case PlayingState: {
            stop();
            break;
        }
        default: {}
    }

    updatePlaylist(0);
    updateItem(0);
}

void Player::updateItem(ModelItem * newItem) {
    emit itemChanged(played, newItem);
    played = newItem;
}
void Player::updatePlaylist(View * newPlaylist) {
    emit playlistChanged(playlist, newPlaylist);
    playlist = newPlaylist;
}

void Player::updateItemState(bool isPlayed) {
    if (played) {
        if (isPlayed) {
            played -> setState(STATE_LISTENED | STATE_PLAYED);
        } else {
            played -> setState(-STATE_PLAYED, false);
        }

        playlist -> getModel() -> refreshItem(played);
    }
}

void Player::playItem(View * itemPlaylist, ModelItem * item) {
    switch(state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            stop();
            break;
        }
    }

    updateItemState(false);

    setMedia(item -> toUrl());

    updatePlaylist(itemPlaylist);
    updateItem(item);
    play();
}

void Player::setTrackBar(QSlider * trackBar) {
    slider = trackBar;
    slider -> setMinimum(0);
    slider -> setMaximum(0);

    connect(this, SIGNAL(positionChanged(int)), this, SLOT(setTrackbarValue(int)));
    connect(this, SIGNAL(durationChanged(int)), this, SLOT(setTrackbarMax(int)));
    connect(trackBar, SIGNAL(valueChanged(int)), this, SLOT(changeTrackbarValue(int)));
}

void Player::setVolumeTrackBar(QSlider * trackBar) {
    volumeSlider = trackBar;
    connect(trackBar, SIGNAL(valueChanged(int)), this, SLOT(setChannelVolume(int)));
    volumeSlider -> setMaximum(10000);
    volumeSlider -> setValue(10000);
}

void Player::setTimePanel(ClickableLabel * newTimePanel) {
    timePanel = newTimePanel;
    connect(timePanel, SIGNAL(clicked()), this, SLOT(invertTimeCountdown()));
}

//void Player::setVideoOutput(QVideoWidget * container) {
//    setVideoOutput(container);
//}

void Player::setTimePanelVal(int millis) {
    if (timePanel) {
        QString val, total;

        if (time_forward) {
            val = intToStr(millis);
        } else {
            val = intToStr(getDuration() - millis);
        }

        total = intToStr(getDuration());
        timePanel -> setText(val + "\n" + total);
    }
}

void Player::initFormat(int millis) {
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
    extended_format = h > 0;
}

QString Player::intToStr(int millis) {
    int m = millis == 0 ? 0 : abs(millis / 60000) % 60;
    int s = millis == 0 ? 0 : abs(millis / 1000) % 60;

    if (extended_format) {
        int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
        return QString().sprintf("%02d:%02d:%02d", h, m, s);
    } else {
        return QString().sprintf("%02d:%02d", m, s);
    }
}

void Player::updateControls(bool played, bool paused, bool stopped) {
    playButton -> setVisible(played);
    pauseButton -> setVisible(paused);
    stopButton -> setVisible(stopped);
    likeButton -> setVisible(!(played && !stopped && !paused));
}

//////////////////////SLOTS/////////////////////////

void Player::invertTimeCountdown() {
    time_forward = !time_forward;
}

void Player::setTrackbarValue(int pos) {
    setTimePanelVal(pos);

    slider -> blockSignals(true);
    slider -> setValue(pos);
    slider -> blockSignals(false);
}

void Player::setTrackbarMax(int duration) {
    if (slider) {
        initFormat(duration);
//        slider -> setDisabled(!isSeekable());
        slider -> setMaximum(duration);
    }
}

void Player::playPause() {
    if (state() == PlayingState)
        pause();
    else
        start();
}

void Player::start() {
    if ((played == 0 || playlist != activePlaylist) && activePlaylist != 0) {
        activePlaylist -> proceedNext();
    } else play();
}

void Player::like() {
    played -> setState(STATE_LIKED);
}

void Player::mute() {
    int curr = getVolume();
    setChannelVolume(prevVolumeVal);
    if ((prevVolumeVal = curr) == 0)
        muteButton -> setIcon(QIcon(":/mute"));
    else
        muteButton -> setIcon(QIcon(":/volume"));
}

void Player::changeTrackbarValue(int pos) {
    emit setPosition(pos);
}

void Player::onStateChanged(MediaState newState) {
    switch(newState) {
        case StoppedState: {
            slider -> blockSignals(true);
            slider -> setMaximum(0);
            setTimePanelVal(0);
            slider -> blockSignals(false);

            updateControls(true, false, false);
            break;
        }

        case PlayingState: {
            if (getDuration() != -1)
                slider -> setMaximum(getDuration());

            updateItemState(true);
            updateControls(false, true, true);
            break;
        }

        case PausedState: {
            updateControls(true, false, true);
            break;
        }
    }
}

void Player::onMediaStatusChanged(MediaStatus status) {
    switch (status) {
        case UnknownMediaStatus: {
        qDebug() << "unk";
        break; }

        case StalledMedia: {
        qDebug() << "sta";
        break; }

        case EndOfMedia:
        case InvalidMedia: {
            qDebug() << "Piza time";
            if (playlist) {
                if (playlist -> isPlaylist()) {
                    playlist -> proceedNext();
                }
            }
            break;
        }
        default: { qDebug() << "Party def"; }
    }
}
