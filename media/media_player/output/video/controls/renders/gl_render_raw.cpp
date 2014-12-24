#include "gl_render_raw.h"

#include <QDebug>

const GLfloat kTexCoords[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1,
};

GLRenderRaw::GLRenderRaw(QWidget* parent) : RenderInterface(parent), shader(0), color_conversion(0) {
    //    setAcceptDrops(true);
    //    /* To rapidly update custom widgets that constantly paint over their entire areas with
    //     * opaque content, e.g., video streaming widgets, it is better to set the widget's
    //     * Qt::WA_OpaquePaintEvent, avoiding any unnecessary overhead associated with repainting the
    //     * widget's background
    //     */
    //    setAttribute(Qt::WA_PaintOnScreen);
    //    setAttribute(Qt::WA_NoSystemBackground);
    //    //default: swap in qpainter dtor. we should swap before QPainter.endNativePainting()
    //    setAutoBufferSwap(false);
    //    setAutoFillBackground(false);




////    setAutoBufferSwap(true);
    setAutoFillBackground(false);
}

GLRenderRaw::~GLRenderRaw() {
    if (shader) {
        delete shader;
        shader = 0;
    }

//    QOpenGLFunctions * f = QOpenGLContext::currentContext() -> functions();
    glDeleteTextures(textures.size(), textures.data());
    textures.clear();

    if (color_conversion) {
        delete color_conversion;
        color_conversion = 0;
    }
}

void GLRenderRaw::setQuality(const Quality & quality) {
//    QOpenGLFunctions * f = QOpenGLContext::currentContext() -> functions();

    switch(quality) {
        case RenderInterface::best : {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        break;}
        case RenderInterface::low : {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
            glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
            //glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
        break;}
        default : {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
            glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
            //glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
        }
    };
}

//void GLRenderRaw::computeOutParameters(qreal outAspectRatio) {
//    qreal rendererAspectRatio = qreal(renderer_width) / qreal(renderer_height);
//    if (out_aspect_ratio_mode == VideoRenderer::RendererAspectRatio) {
//        out_aspect_ratio = rendererAspectRatio;
//        out_rect = QRect(0, 0, renderer_width, renderer_height);
//        return;
//    }

//    if (rendererAspectRatio >= outAspectRatio) {
//        int w = outAspectRatio * qreal(h);
//        out_rect = QRect((renderer_width - w) / 2, 0, w, renderer_height);
//    } else if (rendererAspectRatio < outAspectRatio) {
//        int h = qreal(w) / outAspectRatio;
//        out_rect = QRect(0, (renderer_height - h) / 2, renderer_width, h);
//    }

//    mpv_matrix(0, 0) = (float)out_rect.width()/(float)renderer_width;
//    mpv_matrix(1, 1) = (float)out_rect.height()/(float)renderer_height;
//}

bool GLRenderRaw::initTexture(GLuint tex, GLenum format, GLenum dataType, int width, int height, GLint internalFormat) {
    makeCurrent();
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D
                 , 0                //level
                 , internalFormat
                 , width
                 , height
                 , 0
                 , format
                 , dataType
                 , NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); // Linux ?

//    setQuality(best);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

bool GLRenderRaw::initTextures() {
    makeCurrent();

    //http://www.berkelium.com/OpenGL/GDC99/internalformat.html
    //NV12: UV is 1 plane. 16 bits as a unit. GL_LUMINANCE4, 8, 16, ... 32?
    //GL_LUMINANCE, GL_LUMINANCE_ALPHA are deprecated in GL3, removed in GL3.1
    //replaced by GL_RED, GL_RG, GL_RGB, GL_RGBA? for 1, 2, 3, 4 channel image
    //http://www.gamedev.net/topic/634850-do-luminance-textures-still-exist-to-opengl/
    //https://github.com/kivy/kivy/issues/1738: GL_LUMINANCE does work on a Galaxy Tab 2. LUMINANCE_ALPHA very slow on Linux
     //ALPHA: vec4(1,1,1,A), LUMINANCE: (L,L,L,1), LUMINANCE_ALPHA: (L,L,L,A)
    /*
     * To support both planar and packed use GL_ALPHA and in shader use r,g,a like xbmc does.
     * or use Swizzle_mask to layout the channels: http://www.opengl.org/wiki/Texture#Swizzle_mask
     * GL ES2 support: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_ALPHA
     * http://stackoverflow.com/questions/18688057/which-opengl-es-2-0-texture-formats-are-color-depth-or-stencil-renderable
     */

    VideoSettings * settings = vFrame -> buffer -> settings();

    if (!settings -> isPlanar()) {
        GLint internal_fmt;
        GLenum data_fmt;
        GLenum data_t;

        if (!VideoTypes::videoFormatToGL(settings -> toVideoType(), &internal_fmt, &data_fmt, &data_t)) {
            qWarning("no opengl format found");
            return false;
        }

        //TODO: internal_format did not used at this time
        internal_format = QVector<GLint>(nb_planes, internal_fmt);
        data_format = QVector<GLenum>(nb_planes, data_fmt);
        data_type = QVector<GLenum>(nb_planes, data_t);
    } else {
        internal_format.resize(nb_planes);
        data_format.resize(nb_planes);
        data_type = QVector<GLenum>(nb_planes, GL_UNSIGNED_BYTE);
        if (settings -> isPlanar()) {
            /*!
             * GLES internal_format == data_format, GL_LUMINANCE_ALPHA is 2 bytes
             * so if NV12 use GL_LUMINANCE_ALPHA, YV12 use GL_ALPHA
             */
            internal_format[0] = data_format[0] = GL_LUMINANCE; //or GL_RED for GL
            if (nb_planes == 2) {
                internal_format[1] = data_format[1] = GL_LUMINANCE_ALPHA;
            } else {
                if (settings -> bytesPerPixel(1) == 2) {
                    // read 16 bits and compute the real luminance in shader
                    internal_format.fill(GL_LUMINANCE_ALPHA); //vec4(L,L,L,A)
                    data_format.fill(GL_LUMINANCE_ALPHA);
                } else {
                    internal_format[1] = data_format[1] = GL_LUMINANCE; //vec4(L,L,L,1)
                    internal_format[2] = data_format[2] = GL_ALPHA;//GL_ALPHA;
                }
            }
            for (int i = 0; i < internal_format.size(); ++i) {
                internal_format[i] = VideoTypes::getGLInternalFormat(data_format[i], settings -> bytesPerPixel(i));
                data_format[i] = internal_format[i];
            }
        } else {
            glPixelStorei(GL_UNPACK_ALIGNMENT, settings -> bytesPerPixel());
        }
    }

    textures.resize(nb_planes);
    glGenTextures(textures.size(), textures.data());

    texture_size.resize(nb_planes);

    int bpp_gl;
    for (int i = 0; i < nb_planes; ++i) {
        texture_size[i] = QSize(vFrame -> buffer -> lineSize(i), vFrame -> buffer -> planeHeight(i));

        if (settings -> bytesPerPixel(i) == 2 && nb_planes == 3) { data_type[i] = GL_UNSIGNED_SHORT; }
        bpp_gl = VideoTypes::bytesPerGLFormat(data_format[i], data_type[i]);
        texture_size[i].setWidth(qCeil((qreal)texture_size[i].width() / (qreal)bpp_gl));
    }

    for (int i = 0; i < textures.size(); ++i) {
        initTexture(
                    textures[i],
                    data_format[i],
                    data_type[i],
                    texture_size[i].width(),
                    texture_size[i].height(),
                    internal_format[i]
        );
    }

    return true;
}

void GLRenderRaw::resizeViewport(int w, int h) {
    makeCurrent();

    mpv_matrix(0, 0) = mpv_matrix(1, 1) = 1.0f;

    if (vFrame) {
        output_rect = vFrame -> calcSize(this -> rect());
        glViewport(output_rect.left(), output_rect.top(), output_rect.width(), output_rect.height());
    } else {
        glViewport(0, 0, w, h);
    }
}

void GLRenderRaw::prepareSettings() {
    makeCurrent();

    VideoSettings * settings = vFrame -> buffer -> settings();
    int textures_count;

    nb_planes = settings -> planeCount();
    bipp = settings -> bitsPerPixel(0);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH); //setupQuality?
    glClearDepth(1.0f);

    ColorConversion::ColorSpace cs;
    if (settings -> isRGB()) {
        textures_count = 1;

        if (settings -> isPlanar()) // wtf
            cs = ColorConversion::GBR; // why gbr ?
    } else {
        textures_count = settings -> channelsCount();

        if (settings -> isBT709Colorspace() || (settings -> width >= 1280 && settings -> height > 576))
            cs = ColorConversion::BT709;
        else
            cs = ColorConversion::BT601;
    }

    color_conversion = new ColorConversion(cs, ColorConversion::RGB);

    bool variousEndian = settings -> isPlanar() && settings -> bytesPerPixel(0) == 2;
    shader -> setup(
                textures_count,
                settings -> isPlanar(),
                variousEndian && settings -> isBigEndian(),
                variousEndian && !settings -> isBigEndian()
    );

    initTextures();
}

void GLRenderRaw::initializeGL() {
//    glClearColor(0.0, 0.0, 0.0, 0.0);
    RenderInterface::initializeGL();

//    makeCurrent();

    shader = new Shader(this);

    resizeViewport(QOpenGLWidget::width(), QOpenGLWidget::height());
}

void GLRenderRaw::paintGL() {
    if (vFrame == 0) return;

    makeCurrent();

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    mutex.lock();
    AVPicture * img = vFrame -> asPicture();

    if (init == false) {
        prepareSettings();
        resizeViewport(width(), height());
        init = true;
    }

    shader -> program -> bind();
    QOpenGLFunctions * f = QOpenGLContext::currentContext() -> functions();

    for (int i = 0; i < nb_planes; i++) {
        f -> glActiveTexture(GL_TEXTURE0 + i);
        f -> glBindTexture(GL_TEXTURE_2D, textures[i]);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, texture_size[i].width() + vFrame -> buffer -> pad(i));

        glTexSubImage2D(
                    GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    texture_size[i].width(),
                    texture_size[i].height(),
                    data_format[i],
                    data_type[i],
                    img -> data[i]
        );
//        qDebug() << glGetError();

        shader -> program -> setUniformValue(shader -> u_Texture[i], (GLint)i);
    }

    mutex.unlock();

    if (nb_planes < shader -> u_Texture.size()) {
        for (int i = nb_planes; i < shader -> u_Texture.size(); ++i) {
            shader -> program -> setUniformValue(shader -> u_Texture[i], (GLint)(nb_planes - 1));
        }
    }

    shader -> program -> setUniformValue(shader -> u_colorMatrix, color_conversion -> matrixRef());
    shader -> program -> setUniformValue(shader -> u_matrix, mpv_matrix);
    shader -> program -> setUniformValue(shader -> u_bpp, bipp);

    //   // uniforms done. attributes begin

    shader -> program -> setAttributeArray(shader -> a_Position, GL_FLOAT, kVertices, 2);
    shader -> program -> setAttributeArray(shader -> a_TexCoords, GL_FLOAT, kTexCoords, 2);
    shader -> program -> enableAttributeArray(shader -> a_Position);
    shader -> program -> enableAttributeArray(shader -> a_TexCoords);

    // temporary comented
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    shader -> program -> release();
    shader -> program -> disableAttributeArray(shader -> a_TexCoords);
    shader -> program -> disableAttributeArray(shader -> a_Position);

    redrawed();
}
