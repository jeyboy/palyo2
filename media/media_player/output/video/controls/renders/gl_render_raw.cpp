#include "gl_render_raw.h"

#include <QDebug>

GLRenderRaw::GLRenderRaw(QWidget* parent) : RenderInterface(parent)
  , renderer_width(480)
  , renderer_height(320)
{
    //    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    //    /* To rapidly update custom widgets that constantly paint over their entire areas with
    //     * opaque content, e.g., video streaming widgets, it is better to set the widget's
    //     * Qt::WA_OpaquePaintEvent, avoiding any unnecessary overhead associated with repainting the
    //     * widget's background
    //     */
    //    setAttribute(Qt::WA_OpaquePaintEvent);
    //    setAttribute(Qt::WA_PaintOnScreen);
    //    setAttribute(Qt::WA_NoSystemBackground);
    //    //default: swap in qpainter dtor. we should swap before QPainter.endNativePainting()
    //    setAutoBufferSwap(false);
    //    setAutoFillBackground(false);




////    setAutoBufferSwap(true);
////    setAutoFillBackground(false);

////    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL2);

////    glFormat = GL_RGB;  //  QImage RGBA is BGRA
////    glType = GL_UNSIGNED_BYTE;

    QGLFormat glFmt;
    glFmt.setSwapInterval(1); // 1= vsync on
//    glFmt.setAlpha(GL_RGBA==glFormat);
//    glFmt.setRgba(GL_RGBA==glFormat);
    glFmt.setDoubleBuffer(true); // default
    glFmt.setOverlay(false);
    glFmt.setSampleBuffers(false);
    glFmt.setDepth(false);
//    glFmt.setDirectRendering(true);
    QGLFormat::setDefaultFormat(glFmt);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

GLRenderRaw::~GLRenderRaw() {   
//    if (!textures.isEmpty()) {
//        glDeleteTextures(textures.size(), textures.data());
//        textures.clear();
//    }

    if (shader) {
        delete shader;
        shader = 0;
    }

    if (color_conversion) {
        delete color_conversion;
        color_conversion = 0;
    }
}

void GLRenderRaw::setQuality(const Quality & quality) {
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

void GLRenderRaw::computeOutParameters(qreal outAspectRatio) {
    qreal rendererAspectRatio = qreal(renderer_width) / qreal(renderer_height);
    if (out_aspect_ratio_mode == VideoRenderer::RendererAspectRatio) {
        out_aspect_ratio = rendererAspectRatio;
        out_rect = QRect(0, 0, renderer_width, renderer_height);
        return;
    }

    if (rendererAspectRatio >= outAspectRatio) {
        int w = outAspectRatio * qreal(h);
        out_rect = QRect((renderer_width - w) / 2, 0, w, renderer_height);
    } else if (rendererAspectRatio < outAspectRatio) {
        int h = qreal(w) / outAspectRatio;
        out_rect = QRect(0, (renderer_height - h) / 2, renderer_width, h);
    }

    mpv_matrix(0, 0) = (float)out_rect.width()/(float)renderer_width;
    mpv_matrix(1, 1) = (float)out_rect.height()/(float)renderer_height;
}

bool GLRenderRaw::initTexture(GLuint tex, GLenum format, GLenum dataType, int width, int height, GLint internalFormat) {
    glBindTexture(GL_TEXTURE_2D, tex);
//    setupQuality();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D
                 , 0                //level
                 , internalFormat               //internal format. 4? why GL_RGBA? GL_RGB?
                 , width
                 , height
                 , 0                //border, ES not support
                 , format          //format, must the same as internal format?
                 , dataType
                 , NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

bool GLRenderRaw::initTextures() {
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


    if (!fmt.isPlanar()) {
        GLint internal_fmt;
        GLenum data_fmt;
        GLenum data_t;
        if (!OpenGLHelper::videoFormatToGL(fmt, &internal_fmt, &data_fmt, &data_t)) {
            qWarning("no opengl format found");
            return false;
        }
        internal_format = QVector<GLint>(fmt.planeCount(), internal_fmt);
        data_format = QVector<GLenum>(fmt.planeCount(), data_fmt);
        data_type = QVector<GLenum>(fmt.planeCount(), data_t);
    } else {
        internal_format.resize(fmt.planeCount());
        data_format.resize(fmt.planeCount());
        data_type = QVector<GLenum>(fmt.planeCount(), GL_UNSIGNED_BYTE);
        if (fmt.isPlanar()) {
        /*!
         * GLES internal_format == data_format, GL_LUMINANCE_ALPHA is 2 bytes
         * so if NV12 use GL_LUMINANCE_ALPHA, YV12 use GL_ALPHA
         */
            qDebug("///////////bpp %d", fmt.bytesPerPixel());
            internal_format[0] = data_format[0] = GL_LUMINANCE; //or GL_RED for GL
            if (fmt.planeCount() == 2) {
                internal_format[1] = data_format[1] = GL_LUMINANCE_ALPHA;
            } else {
                if (fmt.bytesPerPixel(1) == 2) {
                    // read 16 bits and compute the real luminance in shader
                    internal_format.fill(GL_LUMINANCE_ALPHA); //vec4(L,L,L,A)
                    data_format.fill(GL_LUMINANCE_ALPHA);
                } else {
                    internal_format[1] = data_format[1] = GL_LUMINANCE; //vec4(L,L,L,1)
                    internal_format[2] = data_format[2] = GL_ALPHA;//GL_ALPHA;
                }
            }
            for (int i = 0; i < internal_format.size(); ++i) {
                // xbmc use bpp not bpp(plane)
                //internal_format[i] = GetGLInternalFormat(data_format[i], fmt.bytesPerPixel(i));
                //data_format[i] = internal_format[i];
            }
        } else {
            //glPixelStorei(GL_UNPACK_ALIGNMENT, fmt.bytesPerPixel());
            // TODO: if no alpha, data_fmt is not GL_BGRA. align at every upload?
        }
    }

    for (int i = 0; i < fmt.planeCount(); ++i) {
        //qDebug("format: %#x GL_LUMINANCE_ALPHA=%#x", data_format[i], GL_LUMINANCE_ALPHA);
        if (fmt.bytesPerPixel(i) == 2 && fmt.planeCount() == 3) {
            //data_type[i] = GL_UNSIGNED_SHORT;
        }
        int bpp_gl = OpenGLHelper::bytesOfGLFormat(data_format[i], data_type[i]);
        int pad = qCeil((qreal)(texture_size[i].width() - effective_tex_width[i])/(qreal)bpp_gl);
        texture_size[i].setWidth(qCeil((qreal)texture_size[i].width()/(qreal)bpp_gl));
        texture_upload_size[i].setWidth(qCeil((qreal)texture_upload_size[i].width()/(qreal)bpp_gl));
        effective_tex_width[i] /= bpp_gl; //fmt.bytesPerPixel(i);
        //effective_tex_width_ratio =
        qDebug("texture width: %d - %d = pad: %d. bpp(gl): %d", texture_size[i].width(), effective_tex_width[i], pad, bpp_gl);
    }

//    /*
//     * there are 2 fragment shaders: rgb and yuv.
//     * only 1 texture for packed rgb. planar rgb likes yuv
//     * To support both planar and packed yuv, and mixed yuv(NV12), we give a texture sample
//     * for each channel. For packed, each (channel) texture sample is the same. For planar,
//     * packed channels has the same texture sample.
//     * But the number of actural textures we upload is plane count.
//     * Which means the number of texture id equals to plane count
//     */
    if (textures.size() != fmt.planeCount()) {
        glDeleteTextures(textures.size(), textures.data());
        qDebug("delete %d textures", textures.size());
        textures.clear();
        textures.resize(fmt.planeCount());
        glGenTextures(textures.size(), textures.data());
    }

    for (int i = 0; i < textures.size(); ++i) {
        initTexture(textures[i], internal_format[i], data_format[i], data_type[i], texture_size[i].width(), texture_size[i].height());
    }
    return true;
}

void GLRenderRaw::resizeViewport(int w, int h) {
    if (vFrame) {
        output_rect = vFrame -> calcSize(this -> rect());
        glViewport(output_rect.left(), output_rect.top(), output_rect.width(), output_rect.height());
    } else {
        glViewport(0, 0, w, h);
    }

    //TODO: recalc aspect ratio matrix
    //computeOutParameters();
}

//void GLRenderRaw::setupTextures(int w, int h) {
//    glEnable(GL_TEXTURE_2D);

//    /* Select texture unit 1 as the active unit and bind the U texture. */
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 1);
//    shader -> program -> setUniformValue(shader -> Utex, 1);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//    glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 376, 288, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Utex);

//    /* Select texture unit 2 as the active unit and bind the V texture. */
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 2);
//    shader -> program -> setUniformValue(shader -> Vtex, 2);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//    glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 376, 288, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Vtex);

//    /* Select texture unit 0 as the active unit and bind the Y texture. */
//    glActiveTexture(GL_TEXTURE0);
//    shader -> program -> setUniformValue(shader -> Ytex, 0);
//    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//    glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 752, 576, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Ytex);
//}

void GLRenderRaw::prepareSettings() {
    mpv_matrix(0, 0) = 1.0f;
    mpv_matrix(1, 1) = 1.0f;

    ColorConversion::ColorSpace cs;
    VideoSettings * settings = vFrame -> buffer -> settings();

    bool variousEndian = settings -> isPlanar() && settings -> bytesPerPixel(0) == 2;
    shader -> setup(settings -> isPlanar(), variousEndian && settings -> isBigEndian(), variousEndian && !settings -> isBigEndian());

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH); //setupQuality?
    glClearDepth(1.0f);

    if (settings -> isRGB()) {
        u_Texture.resize(1);

        if (settings -> isPlanar())
            cs = ColorConversion::GBR;
    } else {
        u_Texture.resize(fmt.channels());

        if (settings -> buff_width >= 1280 || settings -> buff_height() > 576) //values from mpv
            cs = ColorConversion::BT709;
        else
            cs = ColorConversion::BT601;
    }

    QString tex_var;
    for (int i = 0; i < u_Texture.size(); ++i) {
        tex_var = QString("u_Texture%1").arg(i);
        u_Texture[i] = shader -> program -> uniformLocation(tex_var);
    }

    color_conversion = new ColorConversion(cs, ColorConversion::RGB);




    //    // TODO: only to kinds, packed.glsl, planar.glsl
    //    QString frag;
    //    if (fmt.isPlanar()) {
    //        frag = getShaderFromFile("shaders/planar.f.glsl");
    //    } else {
    //        frag = getShaderFromFile("shaders/rgb.f.glsl");
    //    }
    //    if (frag.isEmpty())
    //        return false;
    //    if (fmt.isRGB()) {
    //        frag.prepend("#define INPUT_RGB\n");
    //    } else {
    //        frag.prepend(QString("#define YUV%1P\n").arg(fmt.bitsPerPixel(0)));
    //    }
    //    if (fmt.isPlanar() && fmt.bytesPerPixel(0) == 2) {
    //        if (fmt.isBigEndian())
    //            frag.prepend("#define LA_16BITS_BE\n");
    //        else
    //            frag.prepend("#define LA_16BITS_LE\n");
    //    }
    //    if (cs == ColorTransform::BT601) {
    //        frag.prepend("#define CS_BT601\n");
    //    } else if (cs == ColorTransform::BT709) {
    //        frag.prepend("#define CS_BT709\n");
    //    }
}

void GLRenderRaw::initializeGL() {
    initializeOpenGLFunctions();

    shader = new Shader(context());

    glViewport(0, 0, QGLWidget::width(), QGLWidget::height());
}

void GLRenderRaw::paintGL() {
    if (vFrame == 0) return;

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);


    //    QRect roi = realROI();
    //    const int nb_planes = d.video_frame.planeCount(); //number of texture id
    //    int mapped = 0;
    //    for (int i = 0; i < nb_planes; ++i) {
    //        if (d.video_frame.map(GLTextureSurface, &d.textures[i])) {
    //            OpenGLHelper::glActiveTexture(GL_TEXTURE0 + i);
    //            // if mapped by SurfaceInterop, the texture may be not bound
    //            glBindTexture(GL_TEXTURE_2D, d.textures[i]); //we've bind 0 after upload()
    //            mapped++;
    //        }
    //    }
    //    if (mapped < nb_planes) {
    //        d.upload(roi);
    //    }
    //    //TODO: compute kTexCoords only if roi changed
    //#if ROI_TEXCOORDS
    ///*!
    //  tex coords: ROI/frameRect()*effective_tex_width_ratio
    //*/
    //    const GLfloat kTexCoords[] = {
    //        (GLfloat)roi.x()*(GLfloat)d.effective_tex_width_ratio/(GLfloat)d.video_frame.width(), (GLfloat)roi.y()/(GLfloat)d.video_frame.height(),
    //        (GLfloat)(roi.x() + roi.width())*(GLfloat)d.effective_tex_width_ratio/(GLfloat)d.video_frame.width(), (GLfloat)roi.y()/(GLfloat)d.video_frame.height(),
    //        (GLfloat)(roi.x() + roi.width())*(GLfloat)d.effective_tex_width_ratio/(GLfloat)d.video_frame.width(), (GLfloat)(roi.y()+roi.height())/(GLfloat)d.video_frame.height(),
    //        (GLfloat)roi.x()*(GLfloat)d.effective_tex_width_ratio/(GLfloat)d.video_frame.width(), (GLfloat)(roi.y()+roi.height())/(GLfloat)d.video_frame.height(),
    //    };
    //#else
        const GLfloat kTexCoords[] = {
                0, 0,
                1, 0,
                1, 1,
                0, 1,
        };
    //#endif //ROI_TEXCOORDS

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glScalef((float)d.out_rect.width()/(float)d.renderer_width, (float)d.out_rect.height()/(float)d.renderer_height, 0);
    glVertexPointer(2, GL_FLOAT, 0, kVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, kTexCoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();

    for (int i = 0; i < textures.size(); ++i) {
        video_frame.unmap(&d.textures[i]);
    }

    shader -> program -> bind();

    // all texture ids should be binded when renderering even for packed plane!
    for (int i = 0; i < nb_planes; ++i) {
            // use glUniform1i to swap planes. swap uv: i => (3-i)%3
            // TODO: in shader, use uniform sample2D u_Texture[], and use glUniform1iv(u_Texture, 3, {...})
        shader -> program -> setUniformValue(u_Texture[i], (GLint)i);
    }

    if (nb_planes < u_Texture.size()) {
        for (int i = nb_planes; i < u_Texture.size(); ++i) {
            shader -> program -> setUniformValue(u_Texture[i], (GLint)(nb_planes - 1));
        }
    }

    //    /*
    //     * in Qt4 QMatrix4x4 stores qreal (double), while GLfloat may be float
    //     * QShaderProgram deal with this case. But compares sizeof(QMatrix4x4) and (GLfloat)*16
    //     * which seems not correct because QMatrix4x4 has a flag var
    //     */
    GLfloat *mat = (GLfloat*)d.colorTransform.matrixRef().data();
    GLfloat glm[16];

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        if (sizeof(qreal) != sizeof(GLfloat)) {
    #else
        if (sizeof(float) != sizeof(GLfloat)) {
    #endif
            colorTransform.matrixData(glm);
            mat = glm;
        }

    shader -> program -> setUniformValue(u_colorMatrix, color_conversion -> matrixRef());
    shader -> program -> setUniformValue(u_matrix, mpv_matrix);
    shader -> program -> setUniformValue(u_bpp, (GLfloat)video_format.bitsPerPixel(0));


    //   // uniforms done. attributes begin

    shader -> program -> setAttributeArray(a_Position, GL_FLOAT, kVertices, 2);
    shader -> program -> setAttributeArray(a_TexCoords, GL_FLOAT, kTexCoords, 2);
    shader -> program -> enableAttributeArray(a_Position);
    shader -> program -> enableAttributeArray(a_TexCoords);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    shader -> program -> release();
    shader -> program -> disableAttributeArray(a_TexCoords);
    shader -> program -> disableAttributeArray(a_Position);


    //    for (int i = 0; i < d.textures.size(); ++i) {
    //        d.video_frame.unmap(&d.textures[i]);
    //    }






























    shader -> program -> bind();

    mutex.lock();
//    QImage * img = vFrame -> asImage();

//    if (init == false) {
//        prepareSettings();
//        resizeViewport(width(), height());

//        for (int i = 0; i < 3; i++) {
//    //        glActiveTexture(GL_TEXTURE0 + i);
//            glBindTexture(GL_TEXTURE_2D, yuv_texture_id_[i]);
//            shader -> program -> setUniformValue(yuv_texture_object_[i], (GLint)i);
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, stream_yuv_width_[i], stream_yuv_height_[i], GL_RGBA, GL_UNSIGNED_BYTE, yuv_data_[i]);
//        }
//        glTexImage2D(GL_TEXTURE_2D, 0, 3, img -> width(), img -> height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img -> bits());

//        init = true;
//    } else
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img -> width(), img -> height(), GL_RGB, GL_UNSIGNED_BYTE, img -> bits());

    mutex.unlock();

//    shader -> program -> setAttributeArray(shader -> position_object_, GL_FLOAT, kVertexInformation, 2);
//    shader -> program -> setAttributeArray(shader -> texture_position_object_, GL_FLOAT, kTextureCoordinateInformation, 2);
//    shader -> program -> enableAttributeArray(shader -> position_object_);
//    shader -> program -> enableAttributeArray(shader -> texture_position_object_);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw image (again and again). */

//    glBegin(GL_QUADS);
//      glTexCoord2i(0,0);
//      glVertex2i(0,0);
//      glTexCoord2i(720,0);
//      glVertex2i(B_WIDTH,0);
//      glTexCoord2i(720,576);
//      glVertex2i(B_WIDTH,B_HEIGHT);
//      glTexCoord2i(0,576);
//      glVertex2i(0,B_HEIGHT);
//    glEnd();

    /* Flip buffers. */

//    glFlush();
//    SDL_GL_SwapBuffers();

    shader -> program -> release();
//    shader -> program -> disableAttributeArray(shader -> texture_position_object_);
//    shader -> program -> disableAttributeArray(shader -> position_object_);


//    glEnable(GL_TEXTURE_2D);
//    glDisable(GL_DEPTH_TEST);

//    mutex.lock();
//    glBindTexture(GL_TEXTURE_2D, texture);
//    QImage * img = vFrame -> asImage();

//    if (init == false) {
//        prepareSettings();
//        resizeViewport(width(), height());
//        glTexImage2D(GL_TEXTURE_2D, 0, 3, img -> width(), img -> height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img -> bits());
//        init = true;
//    } else
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img -> width(), img -> height(), GL_RGB, GL_UNSIGNED_BYTE, img -> bits());

//    mutex.unlock();

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
////    glTranslatef(0.0f, 1.0f, 0.0f);

//    glRotatef(180, .5f, 0, 0);

//    glBegin(GL_QUADS);
//        glTexCoord2f(0.0f, 0.0f);  glVertex2f(-1.0f, -1.0f);
//        glTexCoord2f(1.0f, 0.0f);  glVertex2f(1.0f, -1.0f);
//        glTexCoord2f(1.0f, 1.0f);  glVertex2f(1.0f, 1.0f);
//        glTexCoord2f(0.0f, 1.0f);  glVertex2f(-1.0f, 1.0f);
//    glEnd();

    redrawed();
}











////#ifdef GL_EXT_unpack_subimage
//#ifndef GL_UNPACK_ROW_LENGTH
//#ifdef GL_UNPACK_ROW_LENGTH_EXT
//#define GL_UNPACK_ROW_LENGTH GL_UNPACK_ROW_LENGTH_EXT
//#endif //GL_UNPACK_ROW_LENGTH_EXT
//#endif //GL_UNPACK_ROW_LENGTH

//#include "QtAV/ColorTransform.h"
//#include "QtAV/FilterContext.h"

//#define UPLOAD_ROI 0
//#define ROI_TEXCOORDS 1

//#define AVALIGN(x, a) (((x)+(a)-1)&~((a)-1))


////TODO: QGLfunctions?
//namespace QtAV {

//static inline void checkGlError(const char* op = 0) {
//    GLenum error = glGetError();
//    if (error == GL_NO_ERROR)
//        return;
//    qWarning("GL error %s (%#x): %s", op, error, glGetString(error));
//}

//#define CHECK_GL_ERROR(FUNC) \
//    FUNC; \
//    checkGlError(#FUNC);


//class GLWidgetRendererPrivate : public VideoRendererPrivate
//#if QTAV_HAVE(QGLFUNCTIONS)
//        , public QGLFunctions
//#endif //QTAV_HAVE(QGLFUNCTIONS)
//{
//public:
//    GLWidgetRendererPrivate():
//        VideoRendererPrivate()
//    {
//        if (QGLFormat::openGLVersionFlags() == QGLFormat::OpenGL_Version_None) {
//            available = false;
//            return;
//        }
//        colorTransform.setOutputColorSpace(ColorTransform::RGB);
//    }

//    GLuint loadShader(GLenum shaderType, const char* pSource);
//    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
//    bool releaseShaderProgram();
//    QString getShaderFromFile(const QString& fileName);
//    bool prepareShaderProgram(const VideoFormat& fmt, ColorTransform::ColorSpace cs);
//    bool initTexture(GLuint tex, GLint internal_format, GLenum format, GLenum dataType, int width, int height);
//    bool initTextures(const VideoFormat& fmt);
//    void updateTexturesIfNeeded();
//    void upload(const QRect& roi);
//    void uploadPlane(int p, GLint internal_format, GLenum format, const QRect& roi);
//    //GL 4.x: GL_FRAGMENT_SHADER_DERIVATIVE_HINT,GL_TEXTURE_COMPRESSION_HINT
//    //GL_DONT_CARE(default), GL_FASTEST, GL_NICEST
//    /*
//     * it seems that only glTexParameteri matters when drawing an image
//     * MAG_FILTER/MIN_FILTER combinations: http://gregs-blog.com/2008/01/17/opengl-texture-filter-parameters-explained/
//     * TODO: understand what each parameter means. GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T
//     * what is MIPMAP?
//     */

//    void setupAspectRatio() {
//        mpv_matrix(0, 0) = (float)out_rect.width()/(float)renderer_width;
//        mpv_matrix(1, 1) = (float)out_rect.height()/(float)renderer_height;
//    }

//    class VideoMaterialType {};
//    VideoMaterialType* materialType(const VideoFormat& fmt) const {
//        static VideoMaterialType rgbType;
//        static VideoMaterialType packedType; // TODO: uyuy, yuy2
//        static VideoMaterialType planar16leType;
//        static VideoMaterialType planar16beType;
//        static VideoMaterialType yuv8Type;
//        static VideoMaterialType invalidType;
//        if (fmt.isRGB() && !fmt.isPlanar())
//            return &rgbType;
//        if (!fmt.isPlanar())
//            return &packedType;
//        if (fmt.bytesPerPixel(0) == 1)
//            return &yuv8Type;
//        if (fmt.isBigEndian())
//            return &planar16beType;
//        else
//            return &planar16leType;
//        return &invalidType;
//    }
//    void updateShaderIfNeeded();
//};


////http://www.opengl.org/wiki/GLSL#Error_Checking
//// TODO: use QGLShaderProgram for better compatiblity
//GLuint GLWidgetRendererPrivate::loadShader(GLenum shaderType, const char* pSource) {
//    if (!hasGLSL)
//        return 0;
//    GLuint shader = glCreateShader(shaderType);
//    if (shader) {
//        glShaderSource(shader, 1, &pSource, NULL);
//        glCompileShader(shader);
//        GLint compiled = GL_FALSE;
//        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
//        if (compiled == GL_FALSE) {
//            GLint infoLen = 0;
//            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
//            if (infoLen) {
//                char* buf = (char*)malloc(infoLen);
//                if (buf) {
//                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
//                    qWarning("Could not compile shader %d:\n%s\n", shaderType, buf);
//                    free(buf);
//                }
//            }
//            glDeleteShader(shader);
//            shader = 0;
//        }
//    }
//    return shader;
//}

//QString GLWidgetRendererPrivate::getShaderFromFile(const QString &fileName)
//{
//    QFile f(qApp->applicationDirPath() + "/" + fileName);
//    if (!f.exists()) {
//        f.setFileName(":/" + fileName);
//    }
//    if (!f.open(QIODevice::ReadOnly)) {
//        qWarning("Can not load shader %s: %s", f.fileName().toUtf8().constData(), f.errorString().toUtf8().constData());
//        return QString();
//    }
//    QString src = f.readAll();
//    f.close();
//    return src;
//}

//void GLWidgetRendererPrivate::updateTexturesIfNeeded()
//{
//    const VideoFormat &fmt = video_frame.format();
//    bool update_textures = false;
//    if (fmt != video_format) {
//        update_textures = true; //FIXME
//        qDebug("updateTexturesIfNeeded pixel format changed: %s => %s", qPrintable(video_format.name()), qPrintable(fmt.name()));
//    }
//    // effective size may change even if plane size not changed
//    if (update_textures
//            || video_frame.bytesPerLine(0) != plane0Size.width() || video_frame.height() != plane0Size.height()
//            || (plane1_linesize > 0 && video_frame.bytesPerLine(1) != plane1_linesize)) { // no need to check height if plane 0 sizes are equal?
//        update_textures = true;
//        qDebug("---------------------update texture: %dx%d, %s", video_frame.width(), video_frame.height(), video_frame.format().name().toUtf8().constData());
//        const int nb_planes = fmt.planeCount();
//        texture_size.resize(nb_planes);
//        texture_upload_size.resize(nb_planes);
//        effective_tex_width.resize(nb_planes);
//        for (int i = 0; i < nb_planes; ++i) {
//            qDebug("plane linesize %d: padded = %d, effective = %d", i, video_frame.bytesPerLine(i), video_frame.effectiveBytesPerLine(i));
//            qDebug("plane width %d: effective = %d", video_frame.planeWidth(i), video_frame.effectivePlaneWidth(i));
//            qDebug("planeHeight %d = %d", i, video_frame.planeHeight(i));
//            // we have to consider size of opengl format. set bytesPerLine here and change to width later
//            texture_size[i] = QSize(video_frame.bytesPerLine(i), video_frame.planeHeight(i));
//            texture_upload_size[i] = texture_size[i];
//            effective_tex_width[i] = video_frame.effectiveBytesPerLine(i); //store bytes here, modify as width later
//            // TODO: ratio count the GL_UNPACK_ALIGN?
//            //effective_tex_width_ratio = qMin((qreal)1.0, (qreal)video_frame.effectiveBytesPerLine(i)/(qreal)video_frame.bytesPerLine(i));
//        }
//        plane1_linesize = 0;
//        if (nb_planes > 1) {
//            texture_size[0].setWidth(texture_size[1].width() * effective_tex_width[0]/effective_tex_width[1]);
//            // height? how about odd?
//            plane1_linesize = video_frame.bytesPerLine(1);
//        }
//        effective_tex_width_ratio = (qreal)video_frame.effectiveBytesPerLine(nb_planes-1)/(qreal)video_frame.bytesPerLine(nb_planes-1);
//        qDebug("effective_tex_width_ratio=%f", effective_tex_width_ratio);
//        plane0Size.setWidth(video_frame.bytesPerLine(0));
//        plane0Size.setHeight(video_frame.height());
//    }
//    if (update_textures) {
//        initTextures(fmt);
//    }
//}


//void GLWidgetRendererPrivate::upload(const QRect &roi)
//{
//    for (int i = 0; i < video_frame.planeCount(); ++i) {
//        uploadPlane(i, internal_format[i], data_format[i], roi);
//    }
//}

//void GLWidgetRendererPrivate::uploadPlane(int p, GLint internalFormat, GLenum format, const QRect& roi)
//{
//    // FIXME: why happens on win?
//    if (video_frame.bytesPerLine(p) <= 0)
//        return;
//    OpenGLHelper::glActiveTexture(GL_TEXTURE0 + p); //xbmc: only for es, not for desktop?
//    glBindTexture(GL_TEXTURE_2D, textures[p]);
//    ////nv12: 2
//    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//GetAlign(video_frame.bytesPerLine(p)));
//#if defined(GL_UNPACK_ROW_LENGTH)
////    glPixelStorei(GL_UNPACK_ROW_LENGTH, video_frame.bytesPerLine(p));
//#endif
//    //qDebug("bpl[%d]=%d width=%d", p, video_frame.bytesPerLine(p), video_frame.planeWidth(p));
//    // This is necessary for non-power-of-two textures
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    //uploading part of image eats less gpu memory, but may be more cpu(gles)
//    //FIXME: more cpu usage then qpainter. FBO, VBO?
//    //roi for planes?
//    if (ROI_TEXCOORDS || roi.size() == video_frame.size()) {
//        glTexSubImage2D(GL_TEXTURE_2D
//                     , 0                //level
//                     , 0                // xoffset
//                     , 0                // yoffset
//                     , texture_upload_size[p].width()
//                     , texture_upload_size[p].height()
//                     , format          //format, must the same as internal format?
//                     , data_type[p]
//                     , video_frame.bits(p));
//    } else {
//        int roi_x = roi.x();
//        int roi_y = roi.y();
//        int roi_w = roi.width();
//        int roi_h = roi.height();
//        int plane_w = video_frame.planeWidth(p);
//        VideoFormat fmt = video_frame.format();
//        if (p == 0) {
//            plane0Size = QSize(roi_w, roi_h); //
//        } else {
//            roi_x = fmt.chromaWidth(roi_x);
//            roi_y = fmt.chromaHeight(roi_y);
//            roi_w = fmt.chromaWidth(roi_w);
//            roi_h = fmt.chromaHeight(roi_h);
//        }
//        qDebug("roi: %d, %d %dx%d", roi_x, roi_y, roi_w, roi_h);
//#if 0// defined(GL_UNPACK_ROW_LENGTH) && defined(GL_UNPACK_SKIP_PIXELS)
//// http://stackoverflow.com/questions/205522/opengl-subtexturing
//        glPixelStorei(GL_UNPACK_ROW_LENGTH, plane_w);
//        //glPixelStorei or compute pointer
//        glPixelStorei(GL_UNPACK_SKIP_PIXELS, roi_x);
//        glPixelStorei(GL_UNPACK_SKIP_ROWS, roi_y);
//        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, roi_w, roi_h, 0, format, GL_UNSIGNED_BYTE, video_frame.bits(p));
//        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, roi_w, roi_h, format, GL_UNSIGNED_BYTE, video_frame.bits(p));
//        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
//        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
//#else // GL ES
////define it? or any efficient way?
//        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, roi_w, roi_h, 0, format, GL_UNSIGNED_BYTE, NULL);
//        const char *src = (char*)video_frame.bits(p) + roi_y*plane_w + roi_x*fmt.bytesPerPixel(p);
//#define UPLOAD_LINE 1
//#if UPLOAD_LINE
//        for (int y = 0; y < roi_h; y++) {
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, y, roi_w, 1, format, GL_UNSIGNED_BYTE, src);
//        }
//#else
//        int line_size = roi_w*fmt.bytesPerPixel(p);
//        char *sub = (char*)malloc(roi_h*line_size);
//        char *dst = sub;
//        for (int y = 0; y < roi_h; y++) {
//            memcpy(dst, src, line_size);
//            src += video_frame.bytesPerLine(p);
//            dst += line_size;
//        }
//        // FIXME: crash
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, roi_w, roi_h, format, GL_UNSIGNED_BYTE, sub);
//        free(sub);
//#endif //UPLOAD_LINE
//#endif //GL_UNPACK_ROW_LENGTH
//    }
//#if defined(GL_UNPACK_ROW_LENGTH)
////    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//#endif
//    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//}
