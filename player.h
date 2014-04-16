//TODO: improve singleton and add destructor

#ifndef PLAYER_H
#define PLAYER_H

#include <QSlider>
#include <QAction>
#include <QLabel>

#include <QtCore/qmath.h>

#include "model.h"
#include "itemlist.h"
#include "audio_player.h"

class ItemList;

class Player : public AudioPlayer {
    Q_OBJECT
public:

    static Player * instance();
    void playItem(ItemList * itemPlaylist, ModelItem * item);
    void playFile(QString uri);

    void setActivePlaylist(ItemList * itemPlaylist);
    void setPlayButton(QAction * playAction);
    void setPauseButton(QAction * pauseAction);
    void setStopButton(QAction * stopAction);
    void setLikeButton(QAction * likeAction);

    void setTrackBar(QSlider * trackBar);
    void setTimePanel(QLabel * timePanel);
//    void setVideoOutput(QVideoWidget * container);
    void setPlaylist(ItemList * playlist);
    void removePlaylist();

    ModelItem * playedItem() const;
    ItemList * currentPlaylist() const;
    ItemList * currentActivePlaylist() const;

    static void close() {
        delete self;
    }

private slots:
    void start();
    void like();

    void changeTrackbarValue(int);
    void setTrackbarValue(int);
    void setTrackbarMax(int);

    void invertTimeCountdown();

    void onStateChanged(MediaState newState);
    void onMediaStatusChanged(MediaStatus status);

private:
    void updateItemState(bool played);
    void initFormat(int millis);
    QString intToStr(int millis);
    void updateControls(bool played, bool paused, bool stopped);

    void setTimePanelVal(int millis);

    Player() {
        time_forward = true;
        extended_format = true;

        slider = 0;
        timePanel = 0;
        played = 0;
        playlist = 0;

        playButton = 0;
        pauseButton = 0;
        stopButton = 0;
        likeButton = 0;

        activePlaylist = 0;

        setNotifyInterval(500);
        connect(this, SIGNAL(stateChanged(MediaState)), this, SLOT(onStateChanged(MediaState)));
        connect(this, SIGNAL(mediaStatusChanged(MediaStatus)), this, SLOT(onMediaStatusChanged(MediaStatus)));
    }

    static Player * self;
    QSlider * slider;
    QLabel * timePanel;
    ModelItem * played;
    ItemList * playlist;

    ItemList * activePlaylist;

    QAction * playButton;
    QAction * pauseButton;
    QAction * stopButton;
    QAction * likeButton;

    bool time_forward;
    bool extended_format;
};

#endif // PLAYER_H
