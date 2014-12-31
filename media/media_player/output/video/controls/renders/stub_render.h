#ifndef STUB_RENDER_H
#define STUB_RENDER_H

#include "render_interface.h"

class StubRender : public RenderInterface, public QWidget {
public:
    StubRender(int & redrawCounter, QWidget* parent = NULL);
    virtual ~StubRender();

    inline void setFrame(int /*delay*/, VideoFrame * frame) { delete frame; }
    inline void setQuality(const Quality & /*quality*/) {}
    inline RenderType getRenderType() const { return stub; }
    inline QWidget * toWidget() { return this; }
protected:
    void paintEvent(QPaintEvent *);
};

#endif // STUB_RENDER_H
