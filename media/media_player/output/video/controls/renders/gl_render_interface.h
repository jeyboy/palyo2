#ifndef GL_RENDER_INTERFACE_H
#define GL_RENDER_INTERFACE_H

#include "render_types.h"
#include "render_interface.h"

#include <QOpenGLWidget>

class GlRenderInterface : public QOpenGLWidget, public RenderInterface {
public:
    GlRenderInterface(QWidget* parent = NULL);
    virtual ~GlRenderInterface();

    virtual void setQuality(const Quality & quality);
    virtual enum RenderType getRenderType() const = 0;

    inline void repaintNeeded() { QOpenGLWidget::repaint(); }

protected:
    void resizeEvent(QResizeEvent *);
    void resizeGL(int w, int h);
    void initializeGL();
};

#endif // GL_RENDER_INTERFACE_H
