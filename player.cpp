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

void Player::setPlaylist(ItemList * playlist) {
   instance() -> stop();
   instance() -> playlist = playlist;
   instance() -> played = 0;
}

void Player::setPlayedItemState(int state) {
    if (instance() -> played) {
        instance() -> played -> setState(state);
        instance() -> playlist -> model -> refreshItem(instance() -> played);
//        instance() -> palylist -> repaint();
    }
}

void Player::playItem(ItemList * itemPlaylist, ModelItem * item) {
    switch(instance() -> state()) {
        case StoppedState: {
            setPlayedItemState(STATE_LISTENED);
        }

        case PlayingState: {
            instance() -> stop();
            setPlayedItemState(STATE_LISTENED);
        }

        case PausedState: {
            setPlayedItemState(STATE_LISTENED);
        }
    }

    instance() -> setMedia(QUrl::fromLocalFile(item -> fullpath()));

    instance() -> play();
    instance() -> played = item;
    instance() -> playlist = itemPlaylist;
    setPlayedItemState(STATE_PLAYED);
}

void Player::playFile(QString uri) {
    switch(instance() -> state()) {
        case StoppedState: { }

        case PlayingState: {
            instance() -> stop();
            setPlayedItemState(STATE_LISTENED);
        }

        case PausedState: { }
    }

    instance() -> setMedia(QUrl::fromLocalFile(uri));
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
        }

        case PlayingState: { }

        case PausedState: { }
    }
}
