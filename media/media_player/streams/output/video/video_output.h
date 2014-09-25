#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include "media/media_player/utils/video_frame.h"
#include "controls/output_container.h"
#include "controls/gl_output.h"

#include <QLayout>
#include <QWidget>

class VideoOutput : public OutputContainer {
     Q_OBJECT
public:
    VideoOutput(QObject * parent, int width, int height);
    ~VideoOutput();

    void setAspectRatio(int w, int h);
public slots:
    void titleUpdate();
    void setFrame(void * frame);
protected:
    void mouseMoveEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);
//    void paintEvent(QPaintEvent *);
private:
    GLOutput * screen;
};

#endif // VIDEO_OUTPUT_H
