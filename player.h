#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QSlider>

#include "model.h"
#include "itemlist.h"

class ItemList;
class ModelItem;

class Player : public QMediaPlayer {
    Q_OBJECT
public:

    static Player * instance();
    static void playItem(ItemList * itemPlaylist, ModelItem * item);
    static void playFile(QString uri);
    static void setTrackBar(QSlider * trackBar);
    static void setVideoOutput(QVideoWidget * container);
    static void setPlaylist(ItemList * playlist);

    ModelItem * playedItem() const;
    ItemList * currentPlaylist() const;


private slots:
    void changeTrackbarValue(int);
    void setTrackbarValue(qint64);
    void setTrackbarMax(qint64);
    void onStateChanged(QMediaPlayer::State newState);

private:
    static void setPlayedItemState(int state);

    Player() {
       slider = 0;
       setNotifyInterval(500);
       connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));
       played = 0;
    }

    static Player * self;
    QSlider * slider;
    ModelItem * played;
    ItemList * playlist;
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
