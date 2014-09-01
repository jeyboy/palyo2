#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "override/slider.h"
#include "override/slider_style.h"

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    ControlPanel(QWidget * parent = 0);
    QRect getRegion() const;
    void setRegion(QRect rect);
protected slots:
    void sliderUpdate(int);

private:
    QLabel * timer;
    Slider * slider;
    QRect region;
};

#endif // CONTROL_PANEL_H
