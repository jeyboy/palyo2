#include "gl_output.h"

#include <QDebug>
#include <QTime>

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
    glFmt.setSwapInterval(1); // 1= vsync on
//    glFmt.setAlpha(GL_RGBA==glFormat);
//    glFmt.setRgba(GL_RGBA==glFormat);
    glFmt.setDoubleBuffer(true); // default
    glFmt.setOverlay(false);
    glFmt.setSampleBuffers(false);
    QGLFormat::setDefaultFormat(glFmt);

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

    if (output_rect.left() < -100)
        output_rect = frame -> calcSize(this -> rect());

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
//    generate the texture name
    glGenTextures(1, &texture);

    //bind the texture ID
    glBindTexture(GL_TEXTURE_2D, texture);
}

void GLOutput::paintGL() {
////    glClear(GL_COLOR_BUFFER_BIT);
////    glDisable(GL_DEPTH_TEST);
////    glMatrixMode(GL_PROJECTION);
////    glLoadIdentity();
////    gluOrtho2D(0,win.width(),0,win.height());
////    glMatrixMode(GL_MODELVIEW);
////    glLoadIdentity();
//    glEnable(GL_TEXTURE_2D);

    QImage t = QGLWidget::convertToGLFormat(*frame -> image);
//    texture = bindTexture(t);
//    drawTexture(output_rect, texture);
////    deleteTexture(texture);


////    glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////    glFlush();
////    glutSwapBuffers();
////    glDisable(GL_TEXTURE_2D);
///
///
    glEnable(GL_TEXTURE_2D);
//	glAlphaFunc(GL_GREATER, 0.1f);
//	glEnable(GL_ALPHA_TEST);

//    glGenTextures( 1, &texture );
//    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

//    glDisable(GL_ALPHA_TEST);
//    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

//    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void GLOutput::resizeEvent(QResizeEvent * event) {
    QGLWidget::resizeEvent(event);

    output_rect = frame -> calcSize(this -> rect());
}
