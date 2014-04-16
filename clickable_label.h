#ifndef CLICKABLE_LABEL_H
#define CLICKABLE_LABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel {
  Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit ClickableLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
    ~ClickableLabel();

protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void clicked();

};

#endif // CLICKABLE_LABEL_H
