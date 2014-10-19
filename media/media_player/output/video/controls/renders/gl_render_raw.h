#ifndef GL_RENDER_H
#define GL_RENDER_H

#include "media/media_player/utils/shader.h"
#include "media/media_player/output/video/controls/renders/render_interface.h"
#include <QOpenGLFunctions>

static const GLfloat kVertexInformation[] = {
         -1.0f,  1.0f,           // TexCoord 0 top left
         -1.0f, -1.0f,           // TexCoord 1 bottom left
          1.0f, -1.0f,           // TexCoord 2 bottom right
          1.0f,  1.0f            // TexCoord 3 top right
};
static const GLshort kTextureCoordinateInformation[] = {
          0, 0,         // TexCoord 0 top left
          0, 1,         // TexCoord 1 bottom left
          1, 1,         // TexCoord 2 bottom right
          1, 0          // TexCoord 3 top right
};

class GLRenderRaw : public RenderInterface, protected QOpenGLFunctions {
public:
    GLRenderRaw(QWidget* parent = NULL);
    ~GLRenderRaw();

    void setQuality(const Quality & quality);

    inline RenderType getRenderType() const { return gl_plus; }
    void resizeViewport(int w, int h);

    void repaint();

protected:
    void initializeGL();
    void paintGL();  

private:
    Shader * shader;
};

#endif // GL_RENDER_H
