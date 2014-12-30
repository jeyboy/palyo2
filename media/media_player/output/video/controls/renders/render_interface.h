#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "media/media_player/streams/frames/video_frame.h"
#include "render_types.h"

#include <QDebug>
#include <QMutex>

class RenderInterface {
public:
    enum Quality { best, fast, low };

    RenderInterface(int & redrawCounter);
    virtual ~RenderInterface();

    virtual void setQuality(const Quality & quality) = 0;
    virtual enum RenderType getRenderType() const = 0;

    void setFrame(VideoFrame * frame);
    void resize(QRect rect);
    inline void redrawed() { redrawRef++; }
    virtual inline QWidget * toWidget() = 0;
protected:
    bool init, updateRequired;
    int & redrawRef;

    QRect output_rect;
    VideoFrame * vFrame;
    QMutex mutex;
};

#endif // RENDER_INTERFACE_H
