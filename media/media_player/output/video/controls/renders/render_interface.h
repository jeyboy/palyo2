#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "media/media_player/streams/frames/video_frame.h"
#include "render_types.h"

#include <QMutex>
#include <QTimer>
#include <QOpenGLWidget>

class RenderInterface : public QOpenGLWidget {
     Q_OBJECT
public:
    enum Quality { best, fast, low };

    RenderInterface(QWidget* parent = NULL);
    virtual ~RenderInterface();

    virtual void setQuality(const Quality & quality);
    virtual enum RenderType getRenderType() const = 0;

signals:
    void closed();
    void updated();
    void fpsChanged(QString val);
    void frameNeeded(void *&);

private slots:
    void frameInit();

protected:
    void resizeGL(int w, int h);
    void initializeGL();
    void redrawed();

//    virtual void paintFrame() = 0;
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

    bool init, updateRequired;
    int fpsCounter;
    int drawCounter;
    int last_delay;

    QRect output_rect;
    VideoFrame * vFrame;
    QMutex mutex;
    QTimer frameTimer;
};

#endif // RENDER_INTERFACE_H
