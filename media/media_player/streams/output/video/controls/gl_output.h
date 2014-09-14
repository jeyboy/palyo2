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

    void setPauseDelay();
    void setPauseDelay(int millis);
    void setFrame(VideoFrame * frame);

signals:
    void closed();
    void updated();

public slots:
    void drawNext();
protected:
    void closeEvent(QCloseEvent *);
//    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void initializeGL();
    void paintGL();
private:
    QRect output_rect;
    VideoFrame * frame;
    QMutex mutex;
    QTimer timer;

    GLuint texture;
};

#endif // GL_OUTPUT_H
