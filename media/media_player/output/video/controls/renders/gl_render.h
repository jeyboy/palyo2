#ifndef GL_RENDER_H
#define GL_RENDER_H

#include "render_interface.h"
#include <QOpenGLWidget>

class GLRender : public QOpenGLWidget, public RenderInterface {
    Q_OBJECT
public:
    GLRender(int & redrawCounter, QWidget* parent = NULL);
    virtual ~GLRender();

    void setQuality(const Quality & quality);

    inline RenderType getRenderType() const { return gl; }
protected:
    void cleanup();
    void initializeGL();
    void paintGL();
private:
    GLuint texture;
};

#endif // GL_RENDER_H
