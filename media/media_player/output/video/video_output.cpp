#include "video_output.h"
#include "media/media_player/media_player.h"

VideoOutput::VideoOutput(QObject * parent, MasterClock * clock, RenderType type, int width, int height) : OutputContainer(clock)
    , fpsCounter(0)
    , drawCounter(0)
    , last_delay(0)
    , mouse_delay(0)
    , clock(clock)
    , screen(0) {

    resize(width, height);

    QVBoxLayout * newLayout = new QVBoxLayout(this);
    newLayout -> setContentsMargins(0, 0, 0, 0);
    setLayout(newLayout);
    setRender(type);

    panel -> show();
    panel -> raise();

    show();
    setFocus();

    frameInit();
    connect(this, SIGNAL(frameNeeded(void*&)), parent, SLOT(nextFrame(void *&)));
}

VideoOutput::~VideoOutput() {
    delete screen;
}

void VideoOutput::setRender(RenderType type) {
    if (screen) {
        layout() -> removeWidget(screen -> toWidget());
        delete screen;
        screen = 0;
    }

    switch(type) {
        case none: { return; }
        case gl_plus: {
            screen = new GLRenderRaw(drawCounter, this);
            break;
        }
        case gl: {
            screen = new GLRender(drawCounter, this);
            break;
        }
        default: {
            screen = new HardwareRender(drawCounter, this);
            break;
        }
    }

    screen -> toWidget() -> setMouseTracking(true);
    layout() -> addWidget(screen -> toWidget());
    panel -> setRegion(rect());
}

void VideoOutput::titleUpdate() {
    if (screen) {
        QString temp = ((MediaPlayer *)clock -> mediaPlayer()) -> timeInfo() + " (" + renderTypeToStr(screen -> getRenderType()) + " )" + " (" + fps + "fps )" + " (" + (procMemoryUsage(1024 * 1024)) + " Mb )";
        setWindowTitle(temp);
    }
}

void VideoOutput::hideMouse() {
    mouse_delay = 0;
    if (!isFullScreen() || panel -> isVisible()) return;
    QApplication::setOverrideCursor(Qt::BlankCursor);
    offScreenSaver();
}

//void VideoOutput::redrawed() { drawCounter++; }

void VideoOutput::frameInit() {
    if (!screen)
        frameTimer.singleShot((last_delay = 5), this, SLOT(frameInit()));

    if (last_delay != 0) {
        float delay = (1000.0f / last_delay);
        fps = QString::number((int)(drawCounter * delay)) + "d / " + QString::number((int)(fpsCounter * delay)) + "f";
        drawCounter = fpsCounter = 0;
    }

    VideoFrame * frame = 0;
    emit frameNeeded((void *&)frame);
    if (frame) {
        if (!frame -> skip()) {
            fpsCounter++;
            screen -> setFrame(frame);
        }
        frameTimer.singleShot((last_delay = frame -> calcDelay()), this, SLOT(frameInit()));

    } else
        frameTimer.singleShot((last_delay = 5), this, SLOT(frameInit()));

    if ((mouse_delay += last_delay) >= 5000) hideMouse();

    titleUpdate();
}

void VideoOutput::leaveEvent(QEvent *) {
    QApplication::restoreOverrideCursor();
    mouse_delay = 0;
}

void VideoOutput::mouseMoveEvent(QMouseEvent * event) {
    QApplication::restoreOverrideCursor();
    mouse_delay = 0;
    panel -> setVisible(panel -> getRegion().contains(event -> x(), event -> y()));
}

void VideoOutput::resizeEvent(QResizeEvent * event) {
    panel -> setRegion(rect());
    OutputContainer::resizeEvent(event);

    if (screen)
        screen -> resize(rect());
}
