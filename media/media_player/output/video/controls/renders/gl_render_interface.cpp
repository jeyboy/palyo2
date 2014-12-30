#include "gl_render_interface.h"

#include <QDebug>

GlRenderInterface::GlRenderInterface(QWidget* parent) : RenderInterface(parent), QOpenGLWidget(parent) {
//    QOpenGLWidget::setAutoFillBackground(false);
}

GlRenderInterface::~GlRenderInterface() {
    //context is not deleted in usuall way (need to wait 5.4.1 version patch) :(
//    context() -> setParent(this);
}

void GlRenderInterface::resizeGL(int /*w*/, int /*h*/) {

}

void GlRenderInterface::initializeGL() {
    QOpenGLWidget::connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &RenderInterface::cleanup, Qt::DirectConnection);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    //glPixelStorei(GL_UNPACK_SWAP_BYTES,   GL_FALSE);
    //glPixelStorei(GL_UNPACK_LSB_FIRST,    GL_FALSE);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH,   0);
    //glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
    //glPixelStorei(GL_UNPACK_SKIP_ROWS,    0);
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS,  0);
    //glPixelStorei(GL_UNPACK_SKIP_IMAGES,  0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,    1); // 1,2,4,8
}

void GlRenderInterface::setQuality(const Quality & quality) {
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
