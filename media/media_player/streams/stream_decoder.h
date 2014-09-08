#ifndef STREAM_DECODER_H
#define STREAM_DECODER_H

#include "media/media_player/utils/media_player_utils.h"

#include "decoders/audio_stream.h"
#include "decoders/video_stream.h"
#include "decoders/subtitle_stream.h"

class StreamDecoder : public Stream {
    Q_OBJECT
public:
    enum DecoderState {
        NoData = -1,
        Initialization = 0,
        Process = 1,
        EofDetected = 2,
        Seeking = 3,
        Suspended = 4
    };

    StreamDecoder(QObject * parent, AVFormatContext * currContext);
    ~StreamDecoder();

    DecoderState getState() const { return state; }

    inline bool isValid() const { return state == NoData; }
    inline bool isActive() const { return state == Seeking || state == Process || state == Initialization; }
    inline bool isPaused() const { return state == Suspended; }

    inline bool hasAudio() const { return audioStream -> isValid(); }
    inline bool hasVideo() const { return videoStream -> isValid(); }
    inline bool hasSubtitles() const { return subtitleStream -> isValid(); }

    double position();
    void seek(int64_t target);

    void suspend();
    void resume();

    QWidget * getScreenWidget() const;

    uint getVolume() const;
    void setVolume(uint val);
signals:
    void flushDataRequired();
    void eofDetectRequired();
    void eofRejectRequired();
    void suspendRequired();
    void resumeRequired();

//    void errorOccurred(QString message);
//    void stopped();
//    void complete();

protected:
    void routine();

private:
    int langStream();
    uint bestStream(AudioStream * audio, VideoStream * video);
    void findStreams();

    QString defaultLang;
    enum DecoderState state;

    int ac = 0;
    int vc = 0;

    AVFormatContext * context;
    AVPacket * currFrame;

    VideoStream * videoStream;
    AudioStream * audioStream;
    SubtitleStream * subtitleStream;
};

#endif // STREAM_DECODER_H
