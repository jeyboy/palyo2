#include "gl_render_interface.h"

RenderInterface::RenderInterface(QWidget * parent)/* : QOpenGLWidget(parent)*/
  : init(false)
  , vFrame(0) {

//    setFocusPolicy(Qt::StrongFocus);

//    setAttribute(Qt::WA_OpaquePaintEvent, true);
//    frameInit();
}

RenderInterface::~RenderInterface() {
    mutex.lock();
    delete vFrame;
    mutex.unlock();
}

void setFrame(VideoFrame * frame) {
    if (frame) {
        if (!frame -> skip()) {
            mutex.lock();
            delete vFrame;
            vFrame = frame;
            mutex.unlock();
        }
    }
}

//void RenderInterface::resizeGL(int /*w*/, int /*h*/) {

//}

//void RenderInterface::initializeGL() {
//    glClearColor(0.0, 0.0, 0.0, 0.0);

//    //glPixelStorei(GL_UNPACK_SWAP_BYTES,   GL_FALSE);
//    //glPixelStorei(GL_UNPACK_LSB_FIRST,    GL_FALSE);
//    //glPixelStorei(GL_UNPACK_ROW_LENGTH,   0);
//    //glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
//    //glPixelStorei(GL_UNPACK_SKIP_ROWS,    0);
//    //glPixelStorei(GL_UNPACK_SKIP_PIXELS,  0);
//    //glPixelStorei(GL_UNPACK_SKIP_IMAGES,  0);
//    glPixelStorei(GL_UNPACK_ALIGNMENT,    1); // 1,2,4,8
//}

//void RenderInterface::setQuality(const Quality & quality) {
////    QOpenGLFunctions * f = QOpenGLContext::currentContext() -> functions();

//    switch(quality) {
//        case RenderInterface::best : {
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//            //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//        break;}
//        case RenderInterface::low : {
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
//            glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
//            //glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
//            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
//        break;}
//        default : {
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
//            glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
//            //glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
//            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//        }
//    };
//}

////void RenderInterface::repaint() {
////    QWidget::repaint();
////    paintFrame();
////}

//void RenderInterface::redrawed() { drawCounter++; }

//void RenderInterface::frameInit() {
//    if (last_delay != 0) {
//        float delay = (1000.0f / last_delay);
//        emit fpsChanged(QString::number((int)(drawCounter * delay)) + "d / " + QString::number((int)(fpsCounter * delay)) + "f");
//        drawCounter = fpsCounter = 0;
//    }


//    VideoFrame * frame = 0;
//    emit frameNeeded((void *&)frame);
//    if (frame) {
//        if (!frame -> skip()) {
//            fpsCounter++;
//            mutex.lock();
//            delete vFrame;
//            vFrame = frame;
//            mutex.unlock();
//            emit updated();
//            repaint();
//        }
//        frameTimer.singleShot((last_delay = frame -> calcDelay()), this, SLOT(frameInit()));
//    } else frameTimer.singleShot(5, this, SLOT(frameInit()));
//}

//void RenderInterface::closeEvent(QCloseEvent *) {
//    emit closed();
//}

//void RenderInterface::resizeEvent(QResizeEvent * event) {
//    if (vFrame)
//        output_rect = vFrame -> calcSize(this -> rect());
//    else
//        output_rect = this -> rect();

//    QOpenGLWidget::resizeEvent(event);
//}
