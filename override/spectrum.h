#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QToolBar>
#include <QPainter>
#include <QLinearGradient>

class Spectrum : public QToolBar {
    Q_OBJECT

public:
    Spectrum(QWidget *parent = 0);
    ~Spectrum();

    int workHeight();

public slots:
    void bandCountChanged(int newCount);
    void heightChanged(int newHeight);

protected slots:
    void dataUpdated(QList<int>);

protected:
    void paintEvent(QPaintEvent *event);
private:
    QList<int> peaks;
    int bars_count;
};

#endif // SPECTRUM_H
