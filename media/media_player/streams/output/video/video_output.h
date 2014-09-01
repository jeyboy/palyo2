#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include "media/media_player/utils/video_frame.h"
#include "controls/output_container.h"
#include "controls/gl_output.h"
#include "controls/control_panel.h"

#include <QLayout>
#include <QWidget>

class VideoOutput : public OutputContainer {
     Q_OBJECT
public:
    VideoOutput( QObject * parent, int width, int height);
    ~VideoOutput();

    void setFrame(VideoFrame * frame);
public slots:
    void titleUpdate();
protected:
    void mouseMoveEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);
//    void paintEvent(QPaintEvent *);
private:
    GLOutput * screen;
    ControlPanel * panel;
};

#endif // VIDEO_OUTPUT_H
