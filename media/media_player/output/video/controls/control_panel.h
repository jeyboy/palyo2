#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "override/slider.h"
#include "override/slider_style.h"

#include "media/media_player/utils/master_clock.h"

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    ControlPanel(MasterClock * clock, QWidget * parent = 0);
    QRect getRegion() const;
    void setRegion(QRect rect);
protected slots:
    void sliderUpdate(int);

private:
    MasterClock * clock;

    QLabel * timer;
    Slider * slider;
    QRect region;
};

#endif // CONTROL_PANEL_H
