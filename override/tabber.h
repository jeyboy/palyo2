#ifndef TABBER_H
#define TABBER_H

#include <QWidget>
#include <QTabWidget>

#include <data_store.h>
#include "dialogs/tabdialog.h"

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
    Tab * toActiveTab();
    Tab * currentTab();
    Tab * commonTab();
    void load();
    void save();

    void updateIconSize();

private:
    Tab * commonPlaylist;
    QTabWidget * tabber;
    DataStore * store;

private slots:
    void updateActiveTab(QWidget *, QWidget *);
    void handleCurrentChanged(int index);
    void handleTabCloseRequested(int index);
};

#endif // TABBER_H
