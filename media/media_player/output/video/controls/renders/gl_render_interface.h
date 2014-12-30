#ifndef GL_RENDER_INTERFACE_H
#define GL_RENDER_INTERFACE_H

#include <QOpenGLWidget>

class GlRenderInterface {
public:
    GlRenderInterface(QWidget* parent = NULL);
    ~GlRenderInterface();

    virtual void setQuality(const Quality & quality) = 0;
    virtual enum RenderType getRenderType() const = 0;
protected:
    inline QWidget * createScreen(QWidget * parent) {
        return new QOpenGLWidget(parent);
    }
};

#endif // GL_RENDER_INTERFACE_H
