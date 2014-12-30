#ifndef GL_RENDER_INTERFACE_H
#define GL_RENDER_INTERFACE_H

#include "render_types.h"
#include "render_interface.h"

#include <QOpenGLWidget>

class GlRenderInterface : public QOpenGLWidget, virtual public RenderInterface {
public:
    GlRenderInterface(QWidget* parent = NULL);
    virtual ~GlRenderInterface();

    virtual void setQuality(const Quality & quality);
    virtual enum RenderType getRenderType() const = 0;

    inline void repaintNeeded() { QOpenGLWidget::repaint(); }
    inline void resizeNeeded(QResizeEvent * e) {
        QOpenGLWidget::resize(e -> size()); // open gl widget did not sized ???
        QOpenGLWidget::resizeEvent(e);
    }

protected:
    void initializeGL();
};

#endif // GL_RENDER_INTERFACE_H
