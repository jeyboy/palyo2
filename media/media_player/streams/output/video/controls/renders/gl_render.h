#ifndef GL_RENDER_H
#define GL_RENDER_H

#include "media/media_player/streams/output/video/controls/renders/render_interface.h"

#include <QGLWidget>

class GLRender : public QGLWidget, public RenderInterface {
    GLRender(QWidget* parent = NULL);
    ~GLRender();

    void resizeViewport(int w, int h);

protected:
    void initializeGL();
    void paintGL();
private:
    GLuint texture;
};

#endif // GL_RENDER_H
