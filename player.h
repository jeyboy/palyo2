//TODO: improve singleton and add destructor

#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QSlider>
#include <QLCDNumber>
#include <QAction>

#include <QtCore/qmath.h>

#include "model.h"
#include "itemlist.h"

class ItemList;

class Player : public QMediaPlayer {
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
    void setTimePanel(QLCDNumber * timePanel);
    void setVideoOutput(QVideoWidget * container);
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
    void setTrackbarValue(qint64);
    void setTrackbarMax(qint64);
    void onStateChanged(QMediaPlayer::State newState);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    void updateItemState(bool played);
//    static void setPlayedItemState(int state);
    QString intToStr(int val);
    void setTimePanelVal(int millis);

    Player() {
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
       connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));
       connect(this, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    }

    static Player * self;
    QSlider * slider;
    QLCDNumber * timePanel;
    ModelItem * played;
    ItemList * playlist;

    ItemList * activePlaylist;

    QAction * playButton;
    QAction * pauseButton;
    QAction * stopButton;
    QAction * likeButton;

    int last_duration;


//    int ref;
//    float max, climax;
};

#endif // PLAYER_H


//QString directory = QFileDialog::getExistingDirectory(this,tr("Select dir for files to import"));
//if(directory.isEmpty())
//    return;
//QDir dir(directory);
//QStringList files = dir.entryList(QStringList() << "*.mp3",QDir::Files);
//QList<QMediaContent> content;
//for(const QString& f:files)
//{
//    content.push_back(QUrl::fromLocalFile(dir.path()+"/" + f));
//    QFileInfo fi(f);
//    ui->listWidget->addItem(fi.fileName());
//}
//playlist->addMedia(content);
//ui->listWidget->setCurrentRow(playlist->currentIndex() != -1? playlist->currentIndex():0);
