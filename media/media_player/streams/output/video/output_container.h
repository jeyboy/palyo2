#ifndef OUTPUT_CONTAINER_H
#define OUTPUT_CONTAINER_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>


class OutputContainer : public QWidget {
     Q_OBJECT
public:
    OutputContainer(QWidget* parent = NULL);
protected:
    bool event(QEvent *);
};

#endif // OUTPUT_CONTAINER_H
