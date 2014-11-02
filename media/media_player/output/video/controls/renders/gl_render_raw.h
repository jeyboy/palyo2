#ifndef GL_RENDER_RAW_H
#define GL_RENDER_RAW_H

#include "media/media_player/utils/video_types.h"
#include "media/media_player/streams/frames/video/shader.h"
#include "media/media_player/streams/frames/video/color_conversion.h"
#include "media/media_player/output/video/controls/renders/render_interface.h"
#include <QOpenGLFunctions>

const GLfloat kVertices[] = {
    -1,  1,
     1,  1,
     1, -1,
    -1, -1,
};

class GLRenderRaw : public RenderInterface, protected QOpenGLFunctions {
public:
    GLRenderRaw(QWidget* parent = NULL);
    virtual ~GLRenderRaw();

    void setQuality(const Quality & quality);

    inline RenderType getRenderType() const { return gl_plus; }

protected:
    bool initTexture(GLuint tex, GLenum format, GLenum dataType, int width, int height, GLint internalFormat = GL_RGBA);
    bool initTextures();

    void resizeViewport(int w, int h);

    void repaint();

//    void computeOutParameters(qreal outAspectRatio);
    void prepareSettings();
    void initializeGL();
    void paintGL();  

private:
    QVector<GLuint> textures;
    QVector<QSize> texture_size;

    QVector<GLint> internal_format;
    QVector<GLenum> data_format;
    QVector<GLenum> data_type;

    QVector<GLint> u_Texture;
    QMatrix4x4 mpv_matrix;

    Shader * shader;
    ColorConversion * color_conversion;
};

#endif // GL_RENDER_RAW_H
