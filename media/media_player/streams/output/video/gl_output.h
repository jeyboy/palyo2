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

public slots:
    void drawNext();
protected:
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *);
private:
    QWidget * bottomPanel;

    VideoFrame * frame;
    QMutex mutex;
    QTimer timer;
};

#endif // GL_OUTPUT_H
