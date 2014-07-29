#ifndef STREAM_DECODER_H
#define STREAM_DECODER_H

#include <QThread>
#include "media_player_utils.h"
#include "stream.h"

class StreamDecoder : public QThread {

public:
    StreamDecoder(AVFormatContext * currContext, QObject * parent);
    ~StreamDecoder();

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

    Stream * videoStream;
    Stream * audioStream;
    Stream * subtitleStream;
};

#endif // STREAM_DECODER_H
