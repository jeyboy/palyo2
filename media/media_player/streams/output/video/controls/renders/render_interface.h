#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "media/media_player/utils/video_frame.h"

#include <QWidget>
#include <QMutex>

class RenderInterface : public QWidget {
     Q_OBJECT
public:
    RenderInterface(QWidget* parent = NULL);
    ~RenderInterface();

    void setFrame(VideoFrame * frame);
    void resizeViewport(int w, int h) = 0;

signals:
    void closed();
    void updated();

//public slots:
//    void repaint();

protected:
//    virtual void paintFrame() = 0;
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
private:
    bool init;

    QRect output_rect;
    VideoFrame * vFrame;
    QMutex mutex;
};

#endif // RENDER_INTERFACE_H
