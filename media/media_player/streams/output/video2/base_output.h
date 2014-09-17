#ifndef BASE_OUTPUT_H
#define BASE_OUTPUT_H

#include "media/media_player/streams/base/stream.h"
#include "video_output.h"

class BaseOutput : public Stream {
    Q_OBJECT
public:
    BaseOutput(QObject * parent, int width, int height);
    ~BaseOutput();
    void flushData();
    void proceedFrame(VideoFrame * frame);
    inline int bufferSize() const { return frames.count();}
    QWidget * widget() const;
signals:
    void framePrepared(void *);
protected:
    void routine();
private:
    VideoOutput * output;
    QList<VideoFrame *> frames;
};

#endif // BASE_OUTPUT_H
