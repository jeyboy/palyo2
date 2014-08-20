#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include "media/media_player/utils/video_frame.h"
#include "controls/output_container.h"
#include "controls/gl_output.h"

#include "override/slider.h"
#include "override/slider_style.h"

#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

class VideoOutput : public OutputContainer {
     Q_OBJECT
public:
    VideoOutput( QObject * parent, int width, int height);
    ~VideoOutput();

    void setFrame(VideoFrame * frame);
public slots:
    void titleUpdate();
    void sliderUpdate(int);
protected:
    void mouseMoveEvent(QMouseEvent * event);
private:
    GLOutput * screen;
    QWidget * bottomPanel;
    QLabel * timer;
    Slider * slider;
};

#endif // VIDEO_OUTPUT_H
