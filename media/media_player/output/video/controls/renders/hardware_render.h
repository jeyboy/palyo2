#ifndef HARDWARE_RENDER_H
#define HARDWARE_RENDER_H

#include "media/media_player/output/video/controls/renders/render_interface.h"

class HardwareRender : public RenderInterface {
public:
    HardwareRender(QWidget* parent = NULL);
    virtual ~HardwareRender();

    void setQuality(const Quality & quality);
    inline RenderType getRenderType() const { return hardware; }
    void resizeViewport(int w, int h) {}

protected:
    void paintEvent(QPaintEvent *);
};

#endif // HARDWARE_RENDER_H
