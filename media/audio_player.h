#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QApplication>
#include <QObject>
#include <QUrl>
#include <QDir>

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"
#include "override/notify_timer.h"

#include "misc/format.h"
#include "duration.h"

void __stdcall endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
void __stdcall endTrackDownloading(HSYNC, DWORD, DWORD, void * user);

// TODO: add proxy realization
//if (MESS(41,BM_GETCHECK,0,0))
//    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,NULL); // disable proxy
//else
//    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,proxy); // enable proxy


class AudioPlayer : public QObject {
    Q_OBJECT

    Q_ENUMS(MediaStateFlags)
    Q_ENUMS(MediaStatusFlags)

    int openChannel(QString path);
    int openRemoteChannel(QString path);
    void closeChannel();
public:
    enum MediaStateFlags {
        StoppedState,
        PlayingState,
        PausedState
    };
    typedef QFlags<MediaStateFlags> MediaState;

    enum MediaStatusFlags {
        UnknownMediaStatus,
        NoMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        EndOfMedia,
        InvalidMedia
    };
    typedef QFlags<MediaStatusFlags> MediaStatus;

    explicit AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    QList<QVector<int> > & getDefaultSpectrum();
    int getCalcSpectrumBandsCount() const;
    int getPosition() const;
    int getDuration() const;
    int getVolume() const;
    float getSize() const;
    float getRemoteFileDownloadPosition();
    float getBpmValue(QUrl uri);

    QHash<QString, QString> getFileInfo(QUrl uri, bool only_bitrate = false);

    int getNotifyInterval();
    void setNotifyInterval(signed int milis);

    void setMedia(QUrl mediaPath);
    void setSpectrumBandsCount(int bandsCount);
    void setSpectrumHeight(int newHeight);
    void setSpectrumFreq(int millis);

    MediaState state() const;

    bool isPlayed() const;
    bool isPaused() const;
    bool isStoped() const;

signals:
    void remoteUnprocessed();

    void volumeChanged(int);
    void playbackEnded();
    void downloadEnded();
    void stateChanged(MediaState);
    void mediaStatusChanged(MediaStatus);
    void spectrumChanged(QList<QVector<int> >);

    void positionChanged(int);
    void durationChanged(int);
//    void volumeChanged(int);

private slots:
    void started();
    void stoped();
    void signalUpdate();
    void calcSpectrum();

public slots:
    void play();
    void pause();
    void resume();
    void stop();
    void endOfPlayback();
    void endOfDownloading();

    void slidePosForward();
    void slidePosBackward();
    void setPosition(int position);

    void slideVolForward();
    void slideVolBackward();
    void setChannelVolume(int val);
    void setVolume(int val);

private:
    float calcBpm(int channel_id);

    QVector<int> getSpectrum();
    QList<QVector<int> > getComplexSpectrum();
    int getBitrate() const;

    QUrl mediaUri;

    float volumeVal;

    int channelsCount;
    int notifyInterval;
    int duration;
    float size;
    float prevDownloadPos;

    int spectrumBandsCount;
    int spectrumHeight;
    int defaultSpectrumLevel;
    QList<QVector<int> > defaultSpectrum;

    MediaState currentState;

    unsigned long chan;
    HSYNC syncHandle, syncDownloadHandle;
    NotifyTimer * notifyTimer;
    NotifyTimer * spectrumTimer;
};

#endif // AUDIO_PLAYER_H
