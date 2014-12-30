#ifndef GL_RENDER_RAW_H
#define GL_RENDER_RAW_H

#include "media/media_player/utils/video_types.h"
#include "media/media_player/streams/frames/video/shader.h"
#include "media/media_player/streams/frames/video/color_conversion.h"
#include "gl_render_interface.h"

const GLfloat kVertices[] = {
    -1,  1,
     1,  1,
     1, -1,
    -1, -1,
};

class GLRenderRaw : public GlRenderInterface {
public:
    GLRenderRaw(QWidget* parent = NULL);
    virtual ~GLRenderRaw();

    inline RenderType getRenderType() const { return gl_plus; }

protected:
    void cleanupResources();
    bool initTexture(GLuint tex, GLenum format, GLenum dataType, int width, int height, GLint internalFormat = GL_RGBA);
    bool initTextures();

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

    QMatrix4x4 mpv_matrix;

    int nb_planes;
    GLfloat bipp;

    Shader * shader;
    ColorConversion * color_conversion;
};

#endif // GL_RENDER_RAW_H
