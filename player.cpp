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
    return instance() -> played;
}
ItemList * Player::currentPlaylist() const {
    return instance() -> playlist;
}

void Player::setActivePlaylist(ItemList * activePlaylist) {
    qDebug() << " active playlist";
    instance() -> activePlaylist = activePlaylist;
}

void Player::setPlayButton(QAction * playAction) {
    instance() -> playButton = playAction;
    instance() -> playButton -> setVisible(true);
//    connect((QObject *)playAction, SIGNAL(triggered(bool)), instance(), SLOT(play()));
    connect((QObject *)playAction, SIGNAL(triggered(bool)), instance(), SLOT(start()));
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

void Player::updateItemState(bool played) {
    if (instance() -> played) {
        if (played) {
            instance() -> played -> setState(STATE_LISTENED | STATE_PLAYED);
        } else {
            instance() -> played -> setState(-STATE_PLAYED);
        }

        qDebug() << instance() -> played -> getState() -> getValue();
        instance() -> playlist -> getModel() -> refreshItem(instance() -> played);
    }
}

//void Player::setPlayedItemState(int state) {
//    instance() -> played -> setState(state);
//    instance() -> playlist -> getModel() -> refreshItem(instance() -> played);
//}

void Player::playItem(ItemList * itemPlaylist, ModelItem * item) {
    switch(instance() -> state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            instance() -> stop();
            break;
        }
    }

    updateItemState(false);

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

    updateItemState(false);

    instance() -> played = 0;
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

void Player::setTimePanel(QLCDNumber * newTimePanel) {
    instance() -> timePanel = newTimePanel;
}

void Player::setVideoOutput(QVideoWidget * container) {
    instance() -> setVideoOutput(container);
}

void Player::setTimePanelVal(int millis) {
    if (instance() -> timePanel) {
        instance() -> timePanel -> display(intToStr(millis - instance() -> last_duration));
    }
}

QString Player::intToStr(int millis) {
    int h = millis == 0 ? 0 : abs(millis / 1440000) % 24;
    int m = millis == 0 ? 0 : abs(millis / 60000) % 60;
    int s = millis == 0 ? 0 : abs(millis / 1000) % 60;
//    int h = millis == 0 ? 0 : abs(millis / 3600000000l) % 24;
//    int m = millis == 0 ? 0 : abs(millis / 60000000) % 60;
//    int s = millis == 0 ? 0 : abs(millis / 1000000) % 60;

    if (h > 0)
        return QString().sprintf("%02d:%02d:%02d", h, m, s);
    else
        return QString().sprintf("%02d:%02d", m, s);
}

//////////////////////SLOTS/////////////////////////

void Player::setTrackbarValue(qint64 pos) {
//    qDebug() << intToStr(pos);
    instance() -> setTimePanelVal(pos);

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

void Player::start() {
    if (instance() -> playedItem() == 0) {
        instance() -> playlist = instance() -> activePlaylist;
        instance() -> activePlaylist -> proceedNext();
    } else play();
}

void Player::changeTrackbarValue(int pos) {
    emit setPosition(pos);
}

void Player::onStateChanged(QMediaPlayer::State newState) {
    switch(newState) {
        case StoppedState: {

//            updateItemState(false);

            instance() -> slider -> blockSignals(true);
            instance() -> slider -> setMaximum(0);
            instance() -> setTimePanelVal(0);
            instance() -> slider -> blockSignals(false);

            instance() -> playButton -> setVisible(true);
            instance() -> pauseButton -> setVisible(false);
            instance() -> stopButton -> setVisible(false);
            break;
        }

        case PlayingState: {
            if (last_duration != -1)
                instance() -> slider -> setMaximum(instance() -> last_duration);

            updateItemState(true);

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
        case UnknownMediaStatus: {
        qDebug() << "unk";
        break; }

        case StalledMedia: {
        qDebug() << "sta";
        break; }

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
