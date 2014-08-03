#ifndef STREAM_DECODER_H
#define STREAM_DECODER_H

#include "media_player_utils.h"

#include "streams/audio_stream.h"
#include "streams/video_stream.h"
#include "streams/subtitle_stream.h"

class StreamDecoder : public Stream {
    Q_OBJECT
public:
    StreamDecoder(AVFormatContext * currContext, QObject * parent = 0);
    ~StreamDecoder();

    inline bool isValid() const { return state; }

    inline bool hasAudio() const { return audioStream -> isValid(); }
    inline bool hasVideo() const { return videoStream -> isValid(); }
    inline bool hasSubtitles() const { return subtitleStream -> isValid(); }

    void suspendOutput();
    void resumeOutput();
signals:
    void errorOccurred(QString message);
    void stopped();
    void finished();

protected:
    void routine();

private:
    uint bestStream(AudioStream * audio, VideoStream * video);
    void findStreams();

    bool state;

    int ac = 0;
    int vc = 0;

    AVFormatContext * context;
    AVPacket * currFrame;

    VideoStream * videoStream;
    AudioStream * audioStream;
    SubtitleStream * subtitleStream;
};

#endif // STREAM_DECODER_H
