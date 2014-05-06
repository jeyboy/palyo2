//TODO: improve singleton and add destructor

#ifndef PLAYER_H
#define PLAYER_H

#include <QSlider>
#include <QAction>

#include <QtCore/qmath.h>

#include "model/model.h"
#include "model/view.h"
#include "audio_player.h"
#include "override/clickable_label.h"

class View;

class Player : public AudioPlayer {
    Q_OBJECT
public:

    static Player * instance();
    void playItem(View * itemPlaylist, ModelItem * item);
    void playFile(QString uri);

    void setActivePlaylist(View * itemPlaylist);
    void setPlayButton(QAction * playAction);
    void setPauseButton(QAction * pauseAction);
    void setStopButton(QAction * stopAction);
    void setLikeButton(QAction * likeAction);

    void setTrackBar(QSlider * trackBar);
    void setTimePanel(ClickableLabel * timePanel);
//    void setVideoOutput(QVideoWidget * container);
    void setPlaylist(View * playlist);
    void removePlaylist();

    ModelItem * playedItem() const;
    View * currentPlaylist() const;
    View * currentActivePlaylist() const;

    static void close() {
        delete self;
    }

public slots:
    void playPause();

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
    ClickableLabel * timePanel;
    ModelItem * played;

    View * playlist;
    View * activePlaylist;

    QAction * playButton;
    QAction * pauseButton;
    QAction * stopButton;
    QAction * likeButton;

    bool time_forward;
    bool extended_format;
};

#endif // PLAYER_H