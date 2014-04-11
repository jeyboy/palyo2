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
ItemList * Player::currentPlaylist() const {
    return playlist;
}
ItemList * Player::currentActivePlaylist() const {
    return playlist;
}


void Player::setActivePlaylist(ItemList * newActivePlaylist) {
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

void  Player::setLikeButton(QAction * likeAction) {
    likeButton = likeAction;
    likeButton -> setVisible(false);
    connect((QObject *)likeAction, SIGNAL(triggered(bool)), this, SLOT(like()));
}

void Player::setPlaylist(ItemList * playlist) {
   stop();
   playlist = playlist;
   played = 0;
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
    playlist = 0;
    played = 0;
}

void Player::updateItemState(bool isPlayed) {
    if (played) {
        if (isPlayed) {
            qDebug() << "+lp " << played -> fullpath();
            played -> setState(STATE_LISTENED | STATE_PLAYED);
        } else {
            qDebug() << "-p " << played -> fullpath();
            played -> setState(-STATE_PLAYED, false);
        }

        playlist -> getModel() -> refreshItem(played);
    }
}

void Player::playItem(ItemList * itemPlaylist, ModelItem * item) {
    switch(state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            stop();
            break;
        }
    }

    updateItemState(false);

    setMedia(QUrl::fromLocalFile(item -> fullpath()));

    played = item;
    playlist = itemPlaylist;
    last_duration = -1;
    play();
}

void Player::playFile(QString uri) {
    switch(state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            stop();
            break;
        }
    }

    updateItemState(false);

    played = 0;
    setMedia(QUrl::fromLocalFile(uri));
    last_duration = -1;
    play();
}

void Player::setTrackBar(QSlider * trackBar) {
    slider = trackBar;
    slider -> setMinimum(0);
    slider -> setMaximum(0);

    //TODO: remove custom methods
    connect(this, SIGNAL(positionChanged(int)), this, SLOT(setTrackbarValue(int)));
    connect(this, SIGNAL(durationChanged(int)), this, SLOT(setTrackbarMax(int)));
    connect(trackBar, SIGNAL(valueChanged(int)), this, SLOT(changeTrackbarValue(int)));
}

void Player::setTimePanel(QLCDNumber * newTimePanel) {
    timePanel = newTimePanel;
}

//void Player::setVideoOutput(QVideoWidget * container) {
//    setVideoOutput(container);
//}

void Player::setTimePanelVal(int millis) {
    if (timePanel) {
        timePanel -> display(intToStr(millis - last_duration));
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

void Player::setTrackbarValue(int pos) {
    setTimePanelVal(pos);

    slider -> blockSignals(true);
    slider -> setValue(pos);
    slider -> blockSignals(false);
}

void Player::setTrackbarMax(int duration) {
//    max = instance() -> metaData("Duration").toFloat();
//    climax = duration;

//    ref = (duration - max) / (max/ instance() ->notifyInterval());

    // TODO: duration is wrong in some case

    if (slider) {
//        slider -> setDisabled(!isSeekable());
        slider -> setMaximum(duration);
        last_duration = duration;
    }
}

void Player::start() {
    if ((played == 0 || playlist != activePlaylist) && activePlaylist != 0) {
        activePlaylist -> proceedNext();
    } else play();
}

void Player::like() {
    played -> setState(STATE_LIKED);
}

void Player::changeTrackbarValue(int pos) {
    emit setPosition(pos);
}

void Player::onStateChanged(MediaState newState) {
    switch(newState) {
        case StoppedState: {

//            updateItemState(false);

            slider -> blockSignals(true);
            slider -> setMaximum(0);
            setTimePanelVal(0);
            slider -> blockSignals(false);

            playButton -> setVisible(true);
            pauseButton -> setVisible(false);
            stopButton -> setVisible(false);
            likeButton -> setVisible(false);
            break;
        }

        case PlayingState: {
            if (last_duration != -1)
                slider -> setMaximum(last_duration);

            updateItemState(true);

            playButton -> setVisible(false);
            pauseButton -> setVisible(true);
            stopButton -> setVisible(true);
            likeButton -> setVisible(true);
            break;
        }

        case PausedState: {
            playButton -> setVisible(true);
            pauseButton -> setVisible(false);
            stopButton -> setVisible(true);
            likeButton -> setVisible(true);
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
