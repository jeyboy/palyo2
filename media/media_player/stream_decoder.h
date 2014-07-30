#ifndef STREAM_DECODER_H
#define STREAM_DECODER_H

#include <QThread>
#include "media_player_utils.h"

#include "streams/audio_stream.h"
#include "streams/video_stream.h"
#include "streams/subtitle_stream.h"

class StreamDecoder : public QThread {
    Q_OBJECT
public:
    StreamDecoder(AVFormatContext * currContext, QObject * parent);
    ~StreamDecoder();

    QAudioFormat prepareAudioFormat();

    inline bool isValid() const { return state; }

    inline bool hasAudio() const { return audioStream -> isValid(); }
    inline bool hasVideo() const { return videoStream -> isValid(); }
    inline bool hasSubtitles() const { return subtitleStream -> isValid(); }
signals:
    void errorOccurred(QString message);
    void stopped();
    void finished();

private:
    uint bestStream(Stream * audio, Stream * video);
    void findStreams();
    bool nextFrame();

    int selectSampleRate(AVCodec *codec);
    int selectChannelLayout(AVCodec *codec);

    bool state;

    AVFormatContext * context;
    AVPacket * currFrame;

    VideoStream * videoStream;
    AudioStream * audioStream;
    SubtitleStream * subtitleStream;
};

#endif // STREAM_DECODER_H
