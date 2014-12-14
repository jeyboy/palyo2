#ifndef OUTPUT_CONTAINER_H
#define OUTPUT_CONTAINER_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>

#include "control_panel.h"

class OutputContainer : public QWidget {
     Q_OBJECT
public:
    OutputContainer(MasterClock * clock, QWidget* parent = NULL);
protected:
    bool event(QEvent *);

    ControlPanel * panel;
};

#endif // OUTPUT_CONTAINER_H
