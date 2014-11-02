#ifndef GL_RENDER_H
#define GL_RENDER_H

#include "media/media_player/output/video/controls/renders/render_interface.h"

class GLRender : public RenderInterface {
public:
    GLRender(QWidget* parent = NULL);
    virtual ~GLRender();

    void setQuality(const Quality & quality);

    inline RenderType getRenderType() const { return gl; }
    void resizeViewport(int w, int h);

    void repaint();

protected:
    void initializeGL();
    void paintGL();
private:
    GLuint texture;
};

#endif // GL_RENDER_H
