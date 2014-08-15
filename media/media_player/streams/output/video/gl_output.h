#ifndef GL_OUTPUT_H
#define GL_OUTPUT_H

#include "media/media_player/utils/video_frame.h"

#include <QMouseEvent>
#include <QKeyEvent>

#include <QGLWidget>
#include <QMutex>
#include <QTimer>

class GLOutput : public QGLWidget {
     Q_OBJECT
public:
    GLOutput(QWidget* parent = NULL);
    ~GLOutput();

    void setFrame(VideoFrame * frame);

public slots:
    void drawNext();
protected:
    void paintEvent(QPaintEvent *);
    bool event(QEvent *);
private:
    QWidget * bottomPanel;

    VideoFrame * frame;
    QMutex mutex;
    QTimer timer;
};

#endif // GL_OUTPUT_H
