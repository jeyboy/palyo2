#ifndef STREAM_DECODER_H
#define STREAM_DECODER_H

#include "media/media_player/utils/media_player_utils.h"

#include "decoders/audio_stream.h"
#include "decoders/video_stream.h"
#include "decoders/subtitle_stream.h"

class StreamDecoder : public Stream {
    Q_OBJECT
public:
    StreamDecoder(AVFormatContext * currContext, QObject * parent = 0);
    ~StreamDecoder();

    inline bool isValid() const { return state; }
    inline bool isFinished() const { return finished; }

    inline bool hasAudio() const { return audioStream -> isValid(); }
    inline bool hasVideo() const { return videoStream -> isValid(); }
    inline bool hasSubtitles() const { return subtitleStream -> isValid(); }

    void suspendOutput();
    void resumeOutput();
signals:
    void errorOccurred(QString message);
    void stopped();
    void complete();

protected:
    void routine();

private:
    QString defaultLang;
    int langStream();
    uint bestStream(AudioStream * audio, VideoStream * video);
    void findStreams();

    bool state, finished;

    int ac = 0;
    int vc = 0;

    AVFormatContext * context;
    AVPacket * currFrame;

    VideoStream * videoStream;
    AudioStream * audioStream;
    SubtitleStream * subtitleStream;
};

#endif // STREAM_DECODER_H
