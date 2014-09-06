#ifndef STREAM_DECODER_H
#define STREAM_DECODER_H

#include "media/media_player/utils/media_player_utils.h"

#include "decoders/audio_stream.h"
#include "decoders/video_stream.h"
#include "decoders/subtitle_stream.h"

#ifndef INT64_MAX
    #define INT64_MAX 9223372036854775807LL
#endif

#ifndef INT64_MIN
    #define INT64_MIN -INT64_MAX
#endif

class StreamDecoder : public Stream {
    Q_OBJECT
public:
    StreamDecoder(QObject * parent, AVFormatContext * currContext);
    ~StreamDecoder();

    inline bool isValid() const { return state; }

    inline bool hasAudio() const { return audioStream -> isValid(); }
    inline bool hasVideo() const { return videoStream -> isValid(); }
    inline bool hasSubtitles() const { return subtitleStream -> isValid(); }

    double position();

    void seek(int64_t target);

    void suspend();
    void resume();
signals:
    void flushDataRequired();
    void eofDetectedRequired();
    void rejectEofRequired();
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
