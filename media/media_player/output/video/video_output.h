#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include "controls/output_container.h"
#include "controls/renders/renders.h"
#include "media/media_player/utils/system_utils.h"

#include <QLayout>
#include <QWidget>

#define BASE_DELAY 5

class VideoOutput : public OutputContainer {
     Q_OBJECT
public:
    VideoOutput(QObject * parent, MasterClock * clock, RenderType type, int width, int height);
    virtual ~VideoOutput();

    void setRender(RenderType type);
    void setAspectRatio(int w, int h);
signals:
    void frameNeeded(void *&);

public slots:
    void titleUpdate();
protected slots:
    void hideMouse();
    void frameInit();

protected:
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);

private:
    MasterClock * clock;
    QString fps;
    RenderInterface * screen;
    bool trimming;

    int fpsCounter;
    int drawCounter;
    int last_delay;
    int mouse_delay;

    QTimer frameTimer;
};

#endif // VIDEO_OUTPUT_H
