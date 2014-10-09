#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "media/media_player/utils/video_frame.h"
#include "render_types.h"
#include "render_quality.h"

#include <QGLWidget>
#include <QMutex>
#include <QTimer>

class RenderInterface : public QGLWidget {
     Q_OBJECT
public:
    RenderInterface(QWidget* parent = NULL);
    ~RenderInterface();

    void setQuality(const enum RenderQuality &) = 0;
    void setFrame(VideoFrame * frame);
    virtual void resizeViewport(int w, int h) = 0;
    virtual enum RenderType getRenderType() const = 0;

signals:
    void closed();
    void updated();
    void fpsChanged(int val);

private slots:
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
};

#endif // RENDER_INTERFACE_H
