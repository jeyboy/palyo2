#ifndef TABBER_H
#define TABBER_H

#include <QWidget>
#include <QTabWidget>
#include <data_store.h>

//#include <QHash>
//typedef QHash <char, int> CBHash;
//Q_DECLARE_METATYPE(CBHash);

#include "override/tab.h"

class Tabber : public QWidget {
    Q_OBJECT
public:
    explicit Tabber(QTabWidget * container);
    ~Tabber();

    int addTab(QString name, CBHash settings);
    Tab * currentTab();
    void load();
    void save();

private:
    void setNoTabsStyle();

    QTabWidget * tabber;
    DataStore * store;

private slots:
    void handleCurrentChanged(int index);
    void handleTabCloseRequested(int index);

//public:
//    explicit MainWindow(QWidget *parent = 0);
//    ~MainWindow();

//    void on_pushButton_clicked();

//    void on_tabWidget_tabCloseRequested(int index);

//private:
//    Ui::MainWindow *ui;
//    QMediaPlayer * player;
};

#endif // TABBER_H
