#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QWidget>
#include <QUrl>
#include <QTimer>

#include "streams/stream_decoder.h"

class MediaPlayer : public QWidget {
    Q_OBJECT
public:

    MediaPlayer(QWidget * parent = 0);
    ~MediaPlayer();

    void tryHu(QUrl url);
    bool play(QUrl url);
    void resume();
    void pause();
    void stop();

    bool tags(QHash<QString, QString> &);

protected slots:
    void newIteration();

protected:
    bool openContext(QUrl url);
    void closeContext();

private:
    StreamDecoder * decoder;

    QTimer * masterClock;

    QWidget * screen;
    bool isRemote;

    AVFormatContext *context;
};

#endif // MEDIA_PLAYER_H
