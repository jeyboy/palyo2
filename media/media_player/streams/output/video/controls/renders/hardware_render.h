#ifndef HARDWARE_RENDER_H
#define HARDWARE_RENDER_H

#include "media/media_player/streams/output/video/controls/renders/render_interface.h"

class HardwareRender : public RenderInterface {
     Q_OBJECT
public:
    HardwareRender(QWidget* parent = NULL);
    ~HardwareRender();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // HARDWARE_RENDER_H
