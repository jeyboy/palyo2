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
    QRect output_rect;
    QImage * img;
    QMutex mutex;
    QTimer timer;

    GLuint texture;

    const GLfloat squareVertices[8] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
    };

    const GLfloat textureVertices[8] = {
//        1.0f, 1.0f,
//        1.0f, 0.0f,
//        0.0f,  1.0f,
//        0.0f,  0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
//        0.0f, 0.8f,
//        1.0f, 0.8f,
//        0.0f, 0.2f,
//        1.0f, 0.2f
    };
};

#endif // GL_OUTPUT_H
