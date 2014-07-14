#ifndef WIDGETS_H
#define WIDGETS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QBoxLayout>

#include "misc/data_store.h"

class Widgets : public QObject {
    Q_OBJECT
public:
    ~Widgets() {

    }

    static Widgets * instance(QObject * parent);
    static void close() {
        delete self;
    }

    void load(QMainWindow * window, QJsonArray & bars);
    void save(QMainWindow * window, DataStore * settings);

    QDockWidget * createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place = Qt::RightDockWidgetArea);
private:
    Widgets(QObject * parent) : QObject(parent) {

    }

    static Widgets *self;
};

#endif // WIDGETS_H
