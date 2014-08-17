#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include "media/media_player/utils/video_frame.h"
#include "output_container.h"
#include "gl_output.h"

#include <QLayout>
#include <QPushButton>
#include <QWidget>

class VideoOutput : public OutputContainer {
     Q_OBJECT
public:
    VideoOutput(int width, int height, QWidget* parent = 0);
    ~VideoOutput();

    void setFrame(VideoFrame * frame);
public slots:
    void titleUpdate();
protected:
    void mouseMoveEvent(QMouseEvent * event);
private:
    GLOutput * screen;
    QWidget * bottomPanel;
};

#endif // VIDEO_OUTPUT_H
