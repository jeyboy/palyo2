#include "gl_output.h"

#include <QDebug>
#include <QTime>
#include <QApplication>

GLOutput::GLOutput(QWidget* parent) : QGLWidget(parent)
  , frame(new VideoFrame) {

//    setAutoBufferSwap(true);
//    setAutoFillBackground(false);
//    setAttribute(Qt::WA_PaintOnScreen);
////    setAttribute(Qt::WA_UpdatesDisabled);
//    setAttribute(Qt::WA_OpaquePaintEvent);

//    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL2);

//    glFormat = GL_RGB;  //  QImage RGBA is BGRA
//    glType = GL_UNSIGNED_BYTE;

    QGLFormat glFmt;
//    glFmt.setSwapInterval(1); // 1= vsync on
//    glFmt.setAlpha(GL_RGBA==glFormat);
//    glFmt.setRgba(GL_RGBA==glFormat);
    glFmt.setDoubleBuffer(true); // default
    glFmt.setOverlay(false);
    glFmt.setSampleBuffers(false);
    glFmt.setDepth(false);
//    glFmt.setDirectRendering(true);
    QGLFormat::setDefaultFormat(glFmt);

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    setAttribute(Qt::WA_OpaquePaintEvent,true);
    setAttribute(Qt::WA_PaintOnScreen,true);

    drawNext();
}

GLOutput::~GLOutput() {
    mutex.lock();
    delete frame;
    mutex.unlock();

    deleteTexture(texture);
}

void GLOutput::setPauseDelay() {
    this -> frame -> pts = -1;
}

void GLOutput::setPauseDelay(int millis) {
    this -> frame -> pts = -millis;
}

void GLOutput::setFrame(VideoFrame * frame) {
    if (frame -> image) {
        QImage * img = new QImage(QGLWidget::convertToGLFormat(*frame -> image));
        delete frame -> image;
        frame -> image = img;
    }

    mutex.lock();
    delete this -> frame;
    this -> frame = frame;    
    mutex.unlock();
}

void GLOutput::drawNext() {
    if (frame -> image == 0) {
        close();
        return;
    }

//    if (output_rect.left() < -100)
//        output_rect = frame -> calcSize(this -> rect());

    emit updated();
    repaint();

    timer.singleShot(frame -> calcDelay(), this, SLOT(drawNext()));
}

void GLOutput::closeEvent(QCloseEvent *) {
    emit closed();
}

//void GLOutput::paintEvent(QPaintEvent *) {
////    QGLWidget::paintEvent(event);

//    QPainter p(this);
////    p.fillRect(this -> rect(), Qt::black);

//    //Set the painter to use a smooth scaling algorithm.
//    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
////    p.setRenderHint(QPainter::Antialiasing, 1);

//    mutex.lock();
//    p.drawImage(output_rect, *frame -> image);
//    mutex.unlock();
//}

void GLOutput::initializeGL() {
    output_rect = frame -> calcSize(this -> rect());
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, 0, 1);

//    generate the texture name
    glGenTextures(1, &texture);

    //bind the texture ID
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void GLOutput::paintGL() {
    glEnable(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, frame -> image -> width(), frame -> image -> height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, frame -> image -> bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glTranslatef(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);  glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);  glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);  glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);  glVertex2f(-1.0f, 1.0f);
    glEnd();

    /* we can mix gl and qpainter.
     * QPainter painter(this);
     * painter.beginNativePainting();
     * gl functions...
     * painter.endNativePainting();
     * swapBuffers();
     */
//    swapBuffers();
}

void GLOutput::resizeEvent(QResizeEvent * event) {
    QGLWidget::resizeEvent(event);

    output_rect = frame -> calcSize(this -> rect());
    glViewport(0, 0, width(), height());
}
