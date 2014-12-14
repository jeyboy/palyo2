#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QWidget>
#include <QUrl>

#include "streams/stream_decoder.h"
#include "utils/master_clock.h"
#include "contexts/custom_context.h"

class StreamDecoder;

class MediaPlayer : public QObject {
    Q_OBJECT
public:
    MediaPlayer(QObject * parent);
    ~MediaPlayer();

    bool open(QUrl url);
    bool openMillis(QUrl url, int position_millis = 0, int duration_millis = (INT64_MAX / 1000));
    bool openMicro(QUrl url, int64_t position_micromillis = 0, int64_t duration_micromillis = INT64_MAX);

    int64_t durationMicro();
    int durationMillis();

    int64_t positionMicro();
    int positionMillis();

    int bitrate();

    QString lastError() const;
    QString filename();

    QString info();
    bool tags(QHash<QString, QString> &);

    bool isPlayed() const;
    bool isPaused() const;

    QWidget * getScreenWidget() const;

signals:
    void errorOccured(QString error);
    void positionChangedMicro(int64_t);
    void positionChangedMillis(int);

public slots:
    void play();
    void resume();
    void pause();
    void stop();

    void seekMicro(int64_t);
    void seekMillis(int);

    //0..10000
    void setVolume(uint val);

protected:
    bool openContext(QUrl & url);
    void closeContext();

private:
    MasterClock * clock;
    StreamDecoder * decoder;

    bool isRemote;
    int64_t item_duration;

    AVFormatContext *context;
    CustomContext * custom_context;

    QString errorStr;
};

#endif // MEDIA_PLAYER_H
