#ifndef GL_RENDER_H
#define GL_RENDER_H

#include "gl_render_interface.h"

class GLRender : public GlRenderInterface {
public:
    GLRender(QWidget* parent = NULL);
    virtual ~GLRender();

    inline RenderType getRenderType() const { return gl; }

protected:
    void cleanupResources();
    void initializeGL();
    void paintGL();
private:
    GLuint texture;
};

#endif // GL_RENDER_H
