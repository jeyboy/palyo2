#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "media/media_player/utils/video_frame.h"
#include "render_types.h"

#include <QWidget>
#include <QMutex>
#include <QTimer>

class RenderInterface : public QWidget {
     Q_OBJECT
public:
    RenderInterface(QWidget* parent = NULL);
    ~RenderInterface();

    void setFrame(VideoFrame * frame);
    virtual void resizeViewport(int w, int h) = 0;
    virtual enum RenderType getRenderType() const = 0;
    virtual void repaint() = 0;

signals:
    void closed();
    void updated();
    void fpsChanged(int val);

//protected slots:
//    void repaint();

protected:
    void redrawed();
    void fpsCalculation();

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
