#ifndef GL_OUTPUT_H
#define GL_OUTPUT_H

#include <QGLWidget>
#include <QImage>
#include <QMutex>

#include <QTimer>

class GLOutput : public QGLWidget {
public:
    GLOutput(QWidget* parent = NULL);
    ~GLOutput();

    void setImage(QImage * image);
protected:
    void paintEvent(QPaintEvent *);
private:
    QList<QImage *> videoBuffer;

    QMutex mutex;
    QTimer timer;

    QImage * img;
    int interval;
};

#endif // GL_OUTPUT_H
