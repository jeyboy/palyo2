#include "gl_render.h"

#include <QDebug>

GLRender::GLRender(QWidget* parent) : RenderInterface(parent) {
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

GLRender::~GLRender() {
    deleteTexture(texture);
}

void GLRender::resizeViewport(int w, int h) {
    if (vFrame) {
        output_rect = vFrame -> calcSize(this -> rect());
        glViewport(output_rect.left(), output_rect.top(), output_rect.width(), h);
    } else {
        glViewport(0, 0, w, h);
    }
}

void GLRender::initializeGL() {
    glViewport(0, 0, QGLWidget::width(), QGLWidget::height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, 0, 1);

//    generate the texture name
    glGenTextures(1, &texture);

    //bind the texture ID
    glBindTexture(GL_TEXTURE_2D, texture);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void GLRender::paintGL() {
    if (vFrame == 0) return;

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    mutex.lock();
    glBindTexture(GL_TEXTURE_2D, texture);
    QImage * img = vFrame -> asImage();

    if (init == false) {
        resizeViewport(width(), height());
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img -> width(), img -> height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img -> bits());
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
//    glTranslatef(0.0f, 1.0f, 0.0f);

    glRotatef(180, .5f, 0, 0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);  glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);  glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);  glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);  glVertex2f(-1.0f, 1.0f);
    glEnd();

    redrawed();

//    QPainter painter(this);
//    painter.beginNativePainting();
//    painter.drawText(5, 5, 40, 20, Qt::AlignCenter, QString::number(fpsCounter));
//    painter.endNativePainting();
//    makeCurrent();
//    swapBuffers();
}
