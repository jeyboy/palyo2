#ifndef GL_RENDER_H
#define GL_RENDER_H

#include "media/media_player/utils/shader.h"
#include "media/media_player/utils/color_conversion.h"
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
    ~GLRenderRaw();

    bool videoFormatToGL(const AVPixelFormat & fmt, GLint* internal_format, GLenum* data_format, GLenum* data_type);
    int bytesOfGLFormat(GLenum format, GLenum dataType);
    GLint GetGLInternalFormat(GLint data_format, int bpp);

    void setQuality(const Quality & quality);
    bool initTexture(GLuint tex, GLenum format, GLenum dataType, int width, int height, GLint internalFormat = GL_RGBA);
    bool GLRenderRaw::initTextures();

    inline RenderType getRenderType() const { return gl_plus; }
    void resizeViewport(int w, int h);

    void repaint();

protected:
    void computeOutParameters(qreal outAspectRatio);
    void prepareSettings();
    void initializeGL();
    void paintGL();  

private:
    //    bool update_texcoords;
    QVector<GLuint> textures; //texture ids. size is plane count
    QVector<QSize> texture_size;
    //    /*
    //     * actually if render a full frame, only plane 0 is enough. other planes are the same as texture size.
    //     * because linesize[0]>=linesize[1]
    //     * uploade size is required when
    //     * 1. y/u is not an integer because of alignment. then padding size of y < padding size of u, and effective size y/u != texture size y/u
    //     * 2. odd size. enlarge y
    //     */
    //    QVector<QSize> texture_upload_size;

    //    QVector<int> effective_tex_width; //without additional width for alignment
    //    qreal effective_tex_width_ratio;
    QVector<GLint> internal_format;
    QVector<GLenum> data_format;
    QVector<GLenum> data_type;
    //    GLuint program;
    //    GLuint vert, frag;

    QVector<GLint> u_Texture;


    //    QPainter *painter;

    //    VideoFormat video_format;
    //    QSize plane0Size;
    //    // width is in bytes. different alignments may result in different plane 1 linesize even if plane 0 are the same
    //    int plane1_linesize;

    QMatrix4x4 mpv_matrix;

    Shader * shader;
    ColorConversion * color_conversion;
    int renderer_width, renderer_height;
};

#endif // GL_RENDER_H
