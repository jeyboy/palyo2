#include "audio_player.h"
#include <QtMath>
#include <QDebug>

//Get the percentage downloaded of an internet file stream, or the buffer level when streaming in blocks.
//QWORD len=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_END); // file/buffer length
//QWORD buf=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_BUFFER); // buffer level
//float progress=buf*100.0/len; // percentage of buffer filled

//void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user)
void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    AudioPlayer * player = static_cast<AudioPlayer *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    AudioPlayer * player = static_cast<AudioPlayer *>(user);
    emit player -> downloadEnded();
}

AudioPlayer::AudioPlayer(QObject * parent) : QObject(parent) {
    qRegisterMetaType<AudioPlayer::MediaStatus>("MediaStatus");
    qRegisterMetaType<AudioPlayer::MediaState>("MediaState");

    // cheat for cross treadhing
    connect(this, SIGNAL(playbackEnded()), this, SLOT(endOfPlayback()));
    connect(this, SIGNAL(downloadEnded()), this, SLOT(endOfDownloading()));

    duration = -1;
    notifyInterval = 100;
    volumeVal = 1.0;
    spectrumHeight = 0;
    spectrumBandsCount = 28;
    spectrumMultiplicity = 3;

    currentState = StoppedState;

    if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
        throw "An incorrect version of BASS.DLL was loaded";
    }

    if (HIWORD(BASS_FX_GetVersion()) != BASSVERSION) {
        throw "An incorrect version of BASS_FX.DLL was loaded";
    }


    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Init error: " << BASS_ErrorGetCode();
//        throw "Cannot initialize device";

    ///////////////////////////////////////////////
    /// load plugins
    ///////////////////////////////////////////////
    QFileInfoList list = QDir(QCoreApplication::applicationDirPath() + "/libs/bass/plugins").entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);

    foreach(QFileInfo file, list) {
        /*int res = */BASS_PluginLoad(file.filePath().toLatin1(), 0);

//        if (res == 0)
//            qDebug() << file.filePath() << BASS_ErrorGetCode();
//        else
//            qDebug() << file.filePath() << res;
    }
    ///////////////////////////////////////////////

    notifyTimer = new NotifyTimer(this);
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    connect(notifyTimer, SIGNAL(started()), this, SLOT(started()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(stoped()));

    spectrumTimer = new NotifyTimer(this);
    connect(spectrumTimer, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
//    spectrumTimer -> start(25); //40 Hz
    spectrumTimer -> start(20);
}

AudioPlayer::~AudioPlayer() {
    BASS_PluginFree(0);
    notifyTimer -> stop();
    delete notifyTimer;

    spectrumTimer -> stop();
    delete spectrumTimer;
}

int AudioPlayer::getPosition() const {
    return BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * 1000;
}

int AudioPlayer::getDuration() const {
    return duration;
}

int AudioPlayer::getNotifyInterval() {
    return notifyInterval;
}
void AudioPlayer::setNotifyInterval(signed int milis) {
    notifyInterval = milis;
    if (notifyTimer -> isActive())
        notifyTimer -> setInterval(notifyInterval);
}

void AudioPlayer::setMedia(QUrl mediaPath) {
    mediaUri = mediaPath;
}

void AudioPlayer::setSpectrumBandsCount(int bandsCount) {
    spectrumBandsCount = bandsCount;
}
void AudioPlayer::setSpectrumHeight(int newHeight) {
    spectrumHeight = newHeight;
}

void AudioPlayer::setSpectrumFreq(int millis) {
    spectrumTimer -> setInterval(millis);
}

void AudioPlayer::setSpectrumMultiplicity(int mult) {
    spectrumMultiplicity = mult;
}

AudioPlayer::MediaState AudioPlayer::state() const {
    return currentState;
}

bool AudioPlayer::isPlayed() const {
    return currentState == PlayingState;
}
bool AudioPlayer::isPaused() const {
    return currentState == PausedState;
}
bool AudioPlayer::isStoped() const {
    return currentState == StoppedState;
}

////////////////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////////////////

int AudioPlayer::openRemoteChannel(QString path) {
    BASS_ChannelStop(chan);
    chan = BASS_StreamCreateURL(path.toStdWString().c_str(), 0, BASS_SAMPLE_FLOAT, NULL, 0);

//    BASS_Encode_Start(channel, "output.wav", BASS_ENCODE_PCM, NULL, 0);

//    BASS_Encode_StartCAFile(channel, 'mp4f', 'aac ', 0, 128000, "output.mp4"); // only macos
//    BASS_Encode_StartCAFile(channel, 'm4af', 'alac', 0, 0, "output.m4a"); // only macos

    if (!chan) {
        int status = BASS_ErrorGetCode();
        if (status == BASS_ERROR_FILEOPEN)// || status == BASS_ERROR_NONET)
            emit remoteUnprocessed();
        qDebug() << "Can't play stream" <<  BASS_ErrorGetCode() << path.toUtf8();
    }
    return chan;
}

int AudioPlayer::openChannel(QString path) {
    BASS_ChannelStop(chan);
    if (!(chan = BASS_StreamCreateFile(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE)))
//    if (!(stream = BASS_StreamCreateFile(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_LOOP))
//        && !(chan = BASS_MusicLoad(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_LOOP | BASS_MUSIC_RAMP | BASS_MUSIC_POSRESET | BASS_MUSIC_STOPBACK | BASS_STREAM_PRESCAN | BASS_MUSIC_AUTOFREE, 1)))
        qDebug() << "Can't play file " <<  BASS_ErrorGetCode() << path.toUtf8();
    return chan;
}

void AudioPlayer::closeChannel() {
//    BASS_ChannelSlideAttribute(chan, BASS_ATTRIB_VOL, 0, 1000);
    BASS_ChannelStop(chan);
    BASS_ChannelRemoveSync(chan, syncHandle);
    BASS_ChannelRemoveSync(chan, syncDownloadHandle);
    BASS_StreamFree(chan);
}

////////////////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////////////////

void AudioPlayer::started() {
    currentState = PlayingState;
    emit stateChanged(PlayingState);
}

void AudioPlayer::stoped() {
    currentState = StoppedState;
}

void AudioPlayer::signalUpdate() {
    int curr_pos = BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * 1000;

    emit positionChanged(curr_pos);
}

void AudioPlayer::calcSpectrum() {
    QList<QVector<int> > res;
    if (spectrumHeight > 0) {
        if (currentState == StoppedState) {
            QVector<int> l;
            res.append(l.fill(-2, spectrumBandsCount));
            QVector<int> l2;
            res.append(l2.fill(-2, spectrumBandsCount));
            emit spectrumChanged(res);
        } else {
//            res.append(getSpectrum());
//            emit spectrumChanged(res);

            emit spectrumChanged(getComplexSpectrum());
        }
    }
}

void AudioPlayer::slidePosForward() {
    if (currentState == PlayingState || currentState == PausedState) {
        int dur = getDuration();
        int pos = getPosition() + dur / 10;
        if (pos < dur)
            setPosition(pos);
    }
}
void AudioPlayer::slidePosBackward() {
    if (currentState == PlayingState || currentState == PausedState) {
        int pos = getPosition() - (getDuration() / 10);
        if (pos < 0) pos = 0;
        setPosition(pos);
    }
}

//0 to 10000
void AudioPlayer::setChannelVolume(int val) {
    volumeVal = val > 0 ? (val / 10000.0) : 0;
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
    emit volumeChanged(val);
}

void AudioPlayer::slideVolForward() {
    int newVolLevel = getVolume() + 1000;
    if (newVolLevel > 10000) newVolLevel = 10000;
    setChannelVolume(newVolLevel);
}
void AudioPlayer::slideVolBackward() {
    int newVolLevel = getVolume() - 1000;
    if (newVolLevel < 0) newVolLevel = 0;
    setChannelVolume(newVolLevel);
}

// 0 to 10000
void AudioPlayer::setVolume(int val) {
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, val);
    emit volumeChanged(val);
}

float AudioPlayer::getSize() const {
    return size;
}

int AudioPlayer::getVolume() const {
    return volumeVal * 10000;
}

//from 0 to 1
float AudioPlayer::getRemoteFileDownloadPosition() {
    if (size == -1) {
        prevDownloadPos = 0;
        DWORD len = BASS_StreamGetFilePosition(chan, BASS_FILEPOS_END);
        size = len + BASS_StreamGetFilePosition(chan, BASS_FILEPOS_START);
    }

    if (prevDownloadPos != 1) {
        prevDownloadPos = ((BASS_StreamGetFilePosition(chan, BASS_FILEPOS_DOWNLOAD)) / size);
    }
    return prevDownloadPos;
}

QHash<QString, QString> AudioPlayer::getRemoteFileInfo(QString uri) {
    QHash<QString, QString> ret;

    int chUID = BASS_StreamCreateURL(uri.toStdWString().c_str(), 0, 0, NULL, 0);

    if (!chUID)
        return ret;

    float time = BASS_ChannelBytes2Seconds(chUID, BASS_ChannelGetLength(chUID, BASS_POS_BYTE)); // playback duration
    DWORD len = BASS_StreamGetFilePosition(chUID, BASS_FILEPOS_END); // file length
    int bitrate = (len / (125 * time) + 0.5); // average bitrate (Kbps)

    ret.insert("duration", Duration::fromSeconds(time));

    BASS_CHANNELINFO info;
    if (BASS_ChannelGetInfo(chUID, &info)) {
        int size = len + BASS_StreamGetFilePosition(chUID, BASS_FILEPOS_START);
        ret.insert("info", Format::toInfo(Format::toUnits(size), bitrate, info.freq, info.chans));
    }

    BASS_StreamFree(chUID);

    return ret;
}

float AudioPlayer::getBpmValue(QUrl uri) {
    int cochan;

    if (uri.isLocalFile())
        cochan = BASS_StreamCreateFile(false, uri.toLocalFile().toStdWString().c_str(), 0, 0, BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_STREAM_PRESCAN | BASS_SAMPLE_MONO);
    else
        cochan = BASS_StreamCreateURL(uri.toString().toStdWString().c_str(), 0, BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_SAMPLE_MONO, NULL, 0);

    if (cochan) {
//        return calcBpm(cochan);

        int playBackDuration = BASS_ChannelBytes2Seconds(cochan, BASS_ChannelGetLength(cochan, BASS_POS_BYTE));

        return BASS_FX_BPM_DecodeGet(cochan,
                              0,
                              playBackDuration,
                              MAKEWORD(20, 180),
                              BASS_FX_FREESOURCE, //BASS_FX_BPM_BKGRND // BASS_FX_BPM_MULT2
                              NULL, NULL);
    } else return 0;
}

// did not work :(
float AudioPlayer::calcBpm(int channel) {
    int ret, C = 1.1;
    int history_lim = 43, bands_count = 32, fft_length = 1024, cadr_len = fft_length / bands_count;
    float history_aprox = 1.0 / history_lim, beats = 0;
    float energy_aprox = (float)bands_count / fft_length;

    QVector<QVector<float> > history;
    history.fill(QVector<float>(), bands_count);

    while(true) {
        float fft[fft_length];
        ret = BASS_ChannelGetData(channel, fft, BASS_DATA_FFT2048); // fft_length * 2
        if (ret == -1) break;
        int fft_pos = 0;

        for (int band = 0; band < bands_count; band++) {
            float energy = 0;

            for(int limit = 0; limit < cadr_len; limit++, fft_pos++)
                energy += fft[fft_pos];

            float history_total = 0;

            for(int history_pos = 0; history_pos < history[band].length(); history_pos++)
                history_total += history[band][history_pos];

            if ((energy_aprox * energy) > (history_aprox * history_total * C))
                beats += 1;

            history[band].append(energy);
            if (history[band].length() > history_lim)
                history[band].removeFirst();
        }

//        delete [] fft;
    }

    int playBackDuration = BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetLength(channel, BASS_POS_BYTE));
    qDebug() << "!!!!!!!!!!!!!!!!BEAT COUNT " << beats;

    if (beats != 0)
        return beats / (playBackDuration / 60.0);
    else return 0;
}

QVector<int> AudioPlayer::getSpectrum() {
    float fft[1024];
    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048);
    QVector<int> res;

    int b0 = 0, x, y;

    for (x = 0; x < spectrumBandsCount; x++) {
        float peak = 0;
        int b1 = qPow(2, x * 10.0 / (spectrumBandsCount - 1));
        if (b1 > 1023) b1 = 1023;
        if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin
        for (; b0 < b1; b0++)
            if (peak < fft[1 + b0])
                peak = fft[1 + b0];

        y = qSqrt(peak) * spectrumMultiplicity * spectrumHeight - 4; // scale it (sqrt to make low values more visible)
        if (y > spectrumHeight) y = spectrumHeight; // cap it

        res.append(y);
    }

    return res;
}

QList<QVector<int> > AudioPlayer::getComplexSpectrum() {
//    int channelsCount = 2, layerLimit = 1024, blockLimit = layerLimit / spectrumBandsCount;
//    float fft[layerLimit * channelsCount];
//    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL);
//    QList<QVector<int> > res;
//    QVector<int> * block = new QVector<int>();
//    int b0 = 0, x, y, bars = spectrumBandsCount * 2;

//    for (x = 0; x < bars; x++) {
//        float peak = 0;
//        int b1 = qPow(2, x * 10.0 / (bars - 1));
//        if (b1 > 1023) b1 = 1023;
//        if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin
//        for (; b0 < b1; b0++)
//            if (peak < fft[1 + b0])
//                peak = fft[1 + b0];

//        y = qSqrt(peak) * spectrumMultiplicity * (spectrumHeight / 2) - 4; // scale it (sqrt to make low values more visible)
//        if (y > spectrumHeight) y = (spectrumHeight / 2); // cap it

//        block -> append(y);
//        if (block -> length() > blockLimit) {
//            res.append(*block);
//            block = new QVector<int>();
//        }
//    }


//    int currLayerLimit, channelsCount = 2, layerLimit = 1024;
//    float fft[layerLimit * channelsCount];
//    qDebug() << BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL);
//    QList<QVector<int> > res;

//    for(int layer = 0; layer < channelsCount; layer++) {
//        int b0 = 0, x, y;
//        QVector<int> layerRes;
//        currLayerLimit = layer * layerLimit;
//        for (x = 0; x < spectrumBandsCount; x++) {
//            float peak = 0;
//            int b1 = qPow(2, x * 10.0 / (spectrumBandsCount - 1)) + currLayerLimit;
//            if (b1 > currLayerLimit - 1) b1 = currLayerLimit + layerLimit - 1;
//            if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin
//            for (; b0 < b1; b0++)
//                if (peak < fft[1 + b0])
//                    peak = fft[1 + b0];

//            y = qSqrt(peak) * spectrumMultiplicity * (spectrumHeight / 2) - 4; // scale it (sqrt to make low values more visible)
//            if (y > spectrumHeight) y = (spectrumHeight / 2); // cap it

//            layerRes.append(y);
//        }
//        res.append(layerRes);
//    }



    int channelsCount = 2, layerLimit = 1024, gLimit = layerLimit * channelsCount;
    float fft[gLimit];
    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL);
    QList<QVector<int> > res;
    QVector<float> peaks;
    int b0 = 0, x, y, z, peakNum;

    for (x = 0; x < channelsCount; x++)
        res.append(QVector<int>());

    for (x = 0; x < spectrumBandsCount; x++) {
        peaks.clear();
        peaks.fill(0, channelsCount);

        int b1 = qPow(2, x * 10.0 / (spectrumBandsCount - 1)) * channelsCount;
        if (b1 > gLimit - 1) b1 = gLimit - 1;
        if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin
        for (; b0 < b1; b0++) {
            peakNum = (1 + b0) % channelsCount;
            if (peaks[peakNum] < fft[1 + b0])
                peaks[peakNum] = fft[1 + b0];
        }

        for (z = 0; z < channelsCount; z++) {
            y = qSqrt(peaks[z]) * spectrumMultiplicity * (spectrumHeight / 3) - 4; // scale it (sqrt to make low values more visible)
            if (y > spectrumHeight) y = (spectrumHeight / 3); // cap it

            res[z].append(y);
        }
    }

    return res;
}

////////////////////////////////////////////////////////////////////////

void AudioPlayer::play() {
    if (currentState == PausedState) {
        resume();
    } else {
        closeChannel();
        if (mediaUri.isEmpty()) {
            emit mediaStatusChanged(NoMedia);
        } else {
            qDebug() << mediaUri.toString();

            if (mediaUri.isLocalFile()) {
                openChannel(mediaUri.toLocalFile());
                size = 0;
                prevDownloadPos = 1;
            } else {
                openRemoteChannel(mediaUri.toString());
                size = -1;
            }

            if (chan) {
                BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
                duration = BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE)) * 1000;
                durationChanged(duration);
                BASS_ChannelPlay(chan, true);
                notifyTimer -> start(notifyInterval);
                //TODO: remove sync and check end of file by timer
                syncHandle = BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &endTrackSync, this);
                syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);
            } else {
                qDebug() << "Can't play file";
            }
        }
    }
}

void AudioPlayer::pause() {
    notifyTimer -> stop();
    BASS_ChannelPause(chan);
    emit stateChanged(PausedState);
    currentState = PausedState;
}

void AudioPlayer::resume() {
    if (!BASS_ChannelPlay(chan, false)) {
        emit mediaStatusChanged(StalledMedia);
        qDebug() << "Error resuming";
    } else {
        notifyTimer -> start(notifyInterval);
    }
}

void AudioPlayer::stop() {
    closeChannel();
    notifyTimer -> stop();
    emit stateChanged(StoppedState);
}

void AudioPlayer::endOfPlayback() {
    stop();
    closeChannel();
    emit mediaStatusChanged(EndOfMedia);
}

void AudioPlayer::endOfDownloading() {
    prevDownloadPos = 1;
}

void AudioPlayer::setPosition(int position) {
    BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, position / 1000.0), BASS_POS_BYTE);
}
