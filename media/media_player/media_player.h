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

    MediaAttributes * getInfo(QUrl url);

    bool open(QUrl url);
    bool openMillis(QUrl url, int position_millis = 0, int duration_millis = 2147483647);
    bool openMicro(QUrl url, int64_t position_micromillis = 0, int64_t duration_micromillis = INT64_MAX);

    int64_t durationMicro();
    int durationMillis();

    int64_t positionMicro();
    int positionMillis();

    int bitrate();

    QString lastError() const;
    QString filename();

    QString timeInfo(QString separator = " / ");

    bool isPlayed() const;
    bool isPaused() const;

    QWidget * getScreenWidget() const;

signals:
    void errorOccured(QString error);
    void positionChangedMicro(int64_t);
    void positionChangedMillis(int);
    void stateChanged(PlayerState);

public slots:
    void play();
    void resume();
    void pause();
    void stop();

    void seekMicro(int64_t);
    void seekMillis(int);

    //0..10000
    void setVolume(uint val);

protected slots:
    void threadSuspended();

protected:
    bool openContext(QUrl & url);
    void closeContext();

private:
    MediaAttributes * attributes;

    MasterClock * clock;
    StreamDecoder * decoder;

    bool isRemote, onlyInfo;
    int64_t item_duration;

    AVFormatContext * context;
    CustomContext * custom_context;

    QString errorStr;
    QSemaphore * semaphore;
};

#endif // MEDIA_PLAYER_H
