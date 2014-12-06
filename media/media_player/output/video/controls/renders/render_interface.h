#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "media/media_player/streams/frames/video_frame.h"
#include "render_types.h"

#include <QGLWidget>
#include <QMutex>
#include <QTimer>

class RenderInterface : public QGLWidget {
     Q_OBJECT
public:
    enum Quality { best, fast, low };

    RenderInterface(QWidget* parent = NULL);
    virtual ~RenderInterface();

    virtual void setQuality(const Quality & quality) = 0;
//    void setFrame(VideoFrame * frame);
    virtual void resizeViewport(int w, int h) = 0;
    virtual enum RenderType getRenderType() const = 0;

signals:
    void closed();
    void updated();
    void fpsChanged(int val);
    void frameNeeded(void *&);

private slots:
    void frameInit();
    void fpsCalculation();

protected:
    void redrawed();

//    virtual void paintFrame() = 0;
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

    bool init;
    int fpsCounter;

    QRect output_rect;
    VideoFrame * vFrame;
    QMutex mutex;
    QTimer timer;
    QTimer frameTimer;
};

#endif // RENDER_INTERFACE_H
