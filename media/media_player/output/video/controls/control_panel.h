#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "override/slider.h"
#include "override/slider_style.h"

#include "media/media_player/utils/master_clock.h"
#include "media/media_player/media_player_state.h"


class ControlPanel : public QFrame {
    Q_OBJECT
public:
    ControlPanel(MasterClock * clock, QWidget * parent = 0);
    QRect getRegion() const;
    void setRegion(QRect rect);
protected slots:
    void sliderUpdate(int);
    void playerStateChanged(PlayerState);
protected:
    void paintEvent(QPaintEvent *);

private:
    MasterClock * clock;

    QLabel * timer;
    Slider * slider;
    QRect region;

    QPushButton * play, * pause, * stop;
};

#endif // CONTROL_PANEL_H
