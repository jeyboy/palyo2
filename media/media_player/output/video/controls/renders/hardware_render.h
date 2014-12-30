#ifndef HARDWARE_RENDER_H
#define HARDWARE_RENDER_H

#include "render_interface.h"

class HardwareRender : public RenderInterface, public QWidget {
public:
    HardwareRender(int & redrawCounter, QWidget* parent = NULL);
    virtual ~HardwareRender();

    void setQuality(const Quality & quality);
    inline RenderType getRenderType() const { return hardware; }

protected:
    void paintEvent(QPaintEvent *);
};

#endif // HARDWARE_RENDER_H
