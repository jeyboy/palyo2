#ifndef GL_OUTPUT_H
#define GL_OUTPUT_H

#include "media/media_player/utils/video_frame.h"

#include <QGLWidget>
#include <QMutex>

class GLOutput : public QGLWidget {
     Q_OBJECT
public:
    GLOutput(QWidget* parent = NULL);
    ~GLOutput();

    void setFrame(VideoFrame * frame);

signals:
    void closed();
    void updated();

protected:
    void closeEvent(QCloseEvent *);
//    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void initializeGL();
    void paintGL();
private:
    bool init;

    QRect output_rect;
    VideoFrame * vFrame;
    QMutex mutex;

    GLuint texture;
};

#endif // GL_OUTPUT_H
