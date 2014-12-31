#include "gl_render.h"

GLRender::GLRender(int & redrawCounter, QWidget* parent) : QOpenGLWidget(parent), RenderInterface(redrawCounter) {

}

GLRender::~GLRender() {
    cleanup();
}

void GLRender::setQuality(const Quality & quality) {
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

void GLRender::cleanup() {
    qDebug() << "GL CLEANUP";
    makeCurrent();
    glDeleteTextures(1, &texture);
    doneCurrent();
}

void GLRender::initializeGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
//    setQuality(best);

    QOpenGLContext::connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLRender::cleanup, Qt::DirectConnection);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void GLRender::paintGL() {
    if (vFrame == 0 || !updateRequired) return;

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(output_rect.left(), output_rect.top(), output_rect.width(), output_rect.height());

    mutex.lock();
    glBindTexture(GL_TEXTURE_2D, texture);
    QImage * img = vFrame -> asImage();

    if (init == false) {
//        glPixelStorei(GL_UNPACK_ALIGNMENT, vFrame -> buffer -> settings() -> bytesPerPixel());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img -> width(), img -> height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img -> bits());
        init = true;
    } else
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img -> width(), img -> height(), GL_RGB, GL_UNSIGNED_BYTE, img -> bits());

    mutex.unlock();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(180, .5f, 0, 0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);  glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);  glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);  glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);  glVertex2f(-1.0f, 1.0f);
    glEnd();

    redrawed();
}
