#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include "controls/output_container.h"
#include "controls/renders/renders.h"
#include "media/media_player/utils/system_utils.h"

#include <QLayout>
#include <QWidget>

class VideoOutput : public OutputContainer {
     Q_OBJECT
public:
    VideoOutput(QObject * parent, MasterClock * clock, RenderType type, int width, int height);
    virtual ~VideoOutput();

    void setRender(RenderType type);
    void setAspectRatio(int w, int h);
public slots:
    void fpsChanged(int fpsVal);
    void titleUpdate();
protected slots:
    void hideMouse();

protected:
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);
private:
    MasterClock * clock;
    QString fps;
    RenderInterface * screen;
    QTimer timer;
    bool trimming;
    QObject * thread;
};

#endif // VIDEO_OUTPUT_H
