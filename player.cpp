#include "player.h"
#include <QDebug>

Player * Player::self = 0;

Player * Player::instance() {
    if(!self)
        self = new Player();
    return self;
}

ModelItem * Player::playedItem() const {
    return instance() -> played;
}
ItemList * Player::currentPlaylist() const {
    return instance() -> playlist;
}

void Player::setPlayButton(QAction * playAction) {
    instance() -> playButton = playAction;
    instance() -> playButton -> setVisible(true);
    connect((QObject *)playAction, SIGNAL(triggered(bool)), instance(), SLOT(play()));
}
void Player::setPauseButton(QAction * pauseAction) {
    instance() -> pauseButton = pauseAction;
    instance() -> pauseButton -> setVisible(false);
    connect((QObject *)pauseAction, SIGNAL(triggered(bool)), instance(), SLOT(pause()));
}
void Player::setStopButton(QAction * stopAction) {
    instance() -> stopButton = stopAction;
    instance() -> stopButton -> setVisible(false);
    connect((QObject *)stopAction, SIGNAL(triggered(bool)), instance(), SLOT(stop()));
}

void Player::setPlaylist(ItemList * playlist) {
   instance() -> stop();
   instance() -> playlist = playlist;
   instance() -> played = 0;
}

void Player::removePlaylist() {
    switch(instance() -> state()) {
        case PausedState:
        case PlayingState: {
            instance() -> stop();
            break;
        }
        default: {}
    }
    instance() -> playlist = 0;
    instance() -> played = 0;

}

void Player::setPlayedItemState(int state) {
    if (instance() -> played) {
        instance() -> played -> setState(state);
        instance() -> playlist -> getModel() -> refreshItem(instance() -> played);
    }
}

void Player::playItem(ItemList * itemPlaylist, ModelItem * item) {
    switch(instance() -> state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            instance() -> stop();
            break;
        }
    }
    setPlayedItemState(STATE_LISTENED);

    instance() -> setMedia(QUrl::fromLocalFile(item -> fullpath()));

    instance() -> played = item;
    instance() -> playlist = itemPlaylist;
    instance() -> last_duration = -1;
    instance() -> play();
}

void Player::playFile(QString uri) {
    switch(instance() -> state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            instance() -> stop();
            break;
        }
    }

    setPlayedItemState(STATE_LISTENED);
    instance() -> setMedia(QUrl::fromLocalFile(uri));
    instance() -> last_duration = -1;
    instance() -> play();
}

void Player::setTrackBar(QSlider * trackBar) {
    instance() -> slider = trackBar;
    instance() -> slider -> setMinimum(0);
    instance() -> slider -> setMaximum(0);
    connect(instance(), SIGNAL(positionChanged(qint64)), instance(), SLOT(setTrackbarValue(qint64)));
    connect(instance(), SIGNAL(durationChanged(qint64)), instance(), SLOT(setTrackbarMax(qint64)));
    connect(trackBar, SIGNAL(valueChanged(int)), instance(), SLOT(changeTrackbarValue(int)));
}

void Player::setVideoOutput(QVideoWidget * container) {
    setVideoOutput(container);
}

//////////////////////SLOTS/////////////////////////

void Player::setTrackbarValue(qint64 pos) {
//    qDebug() << pos;
    instance() -> slider -> blockSignals(true);
    instance() -> slider -> setValue(pos);
    instance() -> slider -> blockSignals(false);
}

void Player::setTrackbarMax(qint64 duration) {
    qDebug() << metaData("Size") << " | " << duration;
    qDebug() << availableMetaData();
//    max = instance() -> metaData("Duration").toFloat();
//    climax = duration;

//    ref = (duration - max) / (max/ instance() ->notifyInterval());

    // TODO: duration is wrong in some case

    if (instance() -> slider) {
        instance() -> slider -> setDisabled(!instance() -> isSeekable());
        instance() -> slider -> setMaximum(duration);
        instance() -> last_duration = duration;
    }
}

void Player::changeTrackbarValue(int pos) {
    emit setPosition(pos);
}

void Player::onStateChanged(QMediaPlayer::State newState) {
    switch(newState) {
        case StoppedState: {
            setPlayedItemState(STATE_LISTENED);
            instance() -> slider -> blockSignals(true);
            instance() -> slider -> setMaximum(0);
            instance() -> slider -> blockSignals(false);

            instance() -> playButton -> setVisible(true);
            instance() -> pauseButton -> setVisible(false);
            instance() -> stopButton -> setVisible(false);
            break;
        }

        case PlayingState: {
            if (last_duration != -1)
                instance() -> slider -> setMaximum(instance() -> last_duration);
            setPlayedItemState(STATE_PLAYED);
            instance() -> playButton -> setVisible(false);
            instance() -> pauseButton -> setVisible(true);
            instance() -> stopButton -> setVisible(true);
            break;
        }

        case PausedState: {
            instance() -> playButton -> setVisible(true);
            instance() -> pauseButton -> setVisible(false);
            instance() -> stopButton -> setVisible(true);
            break;
        }
    }
}

void Player::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch (status) {
        case UnknownMediaStatus: { break; }

        case StalledMedia: { break; }

        case EndOfMedia:
        case InvalidMedia: {
            qDebug() << "Party time";
            if (instance() -> playlist) {
                if (instance() -> playlist -> isPlaylist()) {
                    instance() -> playlist -> proceedNext();
                }
            }
            break;
        }
        default: { qDebug() << "Party def"; }
    }
}
