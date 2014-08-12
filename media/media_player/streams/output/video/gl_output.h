#ifndef GL_OUTPUT_H
#define GL_OUTPUT_H

#include "media/media_player/utils/video_frame.h"
#include <QGLWidget>
#include <QMutex>

#include <QTimer>

class GLOutput : public QGLWidget {
     Q_OBJECT
public:
    GLOutput(QWidget* parent = NULL);
    ~GLOutput();

    void setFrame(VideoFrame * frame);
    double millisPreloaded();

public slots:
    void drawNext();
protected:
    void paintEvent(QPaintEvent *);
private:
    double preloadedMillis;

    VideoFrame * frame;
    QMutex mutex;
    QTimer timer;

    QList<VideoFrame *> videoBuffer;
};

#endif // GL_OUTPUT_H
