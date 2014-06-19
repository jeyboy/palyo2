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
    void dataUpdated(QList<QVector<int> >);

protected:
    void paintEvent(QPaintEvent *event);
    void paintCombo(QPainter & painter);
    void paintDuo(QPainter & painter);

private:
    QList<QVector<int> > peaks;
    int bars_count;
};

#endif // SPECTRUM_H
