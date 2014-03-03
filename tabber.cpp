#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabber.h"
#include <QApplication>
#include <QMessageBox>

void Tabber::handleTabCloseRequested(int index) {
   QMessageBox::information(QApplication::activeWindow(), QString("Удаление"), QString("Удаление таба %1").arg(index));

//   QString("Целое %1 и дробное %2").arg(a).arg(b)

//        QWidget *pWin = QApplication::activeWindow;
//    //    QObjectList *pList = pWin->children();
//        QTabWidget* tabber = pWin->findChild<QTabWidget*>("tabber");
//        tabber -> addTab(tabber, QString('Hudo'));
}

Tabber::Tabber(QTabWidget * container) {
    tabber = container;
    connect(tabber, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseRequested(int)));
    store = new DataStore("tabs.json");
    load();
}

Tabber::~Tabber() {
    delete store;
    delete tabber;
}

int Tabber::addTab(QString name, CBHash settings) {
    return tabber -> addTab(new Tab(settings, tabber), name);
}

Tab * Tabber::currentTab() {
     return static_cast<Tab *>(tabber->currentWidget());
}

void Tabber::save() {
    Player::instance() ->stop();
    store -> clear();
    for(int i = 0; i < tabber->count(); i++) {
        Tab * tab = (Tab*)(tabber -> widget(i));
        store->write(QString::number(i) + "h", tab -> toJSON(tabber -> tabText(i)));
    }

    store -> save();
}

void Tabber::load() {
    if (store->state) {
        QJsonObject tab;

        foreach(QString key, store-> keys()) {
            tab = store -> read(key).toObject();
            Tab * new_tab = new Tab(tab, tabber);
            tabber -> addTab(new_tab, tab["n"].toString());
            new_tab -> updateHeader(tabber);
        }
    }
}
