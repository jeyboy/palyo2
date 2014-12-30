#ifndef GL_RENDER_INTERFACE_H
#define GL_RENDER_INTERFACE_H

#include "render_types.h"
#include "render_interface.h"

#include <QOpenGLWidget>

class GlRenderInterface : public RenderInterface, virtual public QOpenGLWidget {
public:
    GlRenderInterface(QWidget* parent = NULL);
    virtual ~GlRenderInterface();

    virtual void setQuality(const Quality & quality);
    virtual enum RenderType getRenderType() const = 0;

protected:
    void resizeGL(int w, int h);
    void initializeGL();
};

#endif // GL_RENDER_INTERFACE_H
