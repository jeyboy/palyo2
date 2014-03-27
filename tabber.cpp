#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabber.h"
#include <QApplication>
#include <QMessageBox>

//TODO:// add tab position for each tab
//    tabber->setTabPosition(TabPosition);

void Tabber::handleTabCloseRequested(int index) {
    Tab * del_tab = static_cast<Tab *>(tabber->widget(index));
    if (Player::instance() -> currentPlaylist() == del_tab -> getList()) {
        Player::instance() -> removePlaylist();
    }

    tabber -> removeTab(index);
}

void Tabber::showContextMenu(const QPoint& pnt) {
    QList<QAction *> actions;
//    if (tabber -> indexAt(pnt).isValid()) {
//        actions.append(m_addAction);
//    }
//    if (actions.count() > 0)
    QMenu::exec(actions, tabber -> mapToGlobal(pnt));
}

Tabber::Tabber(QTabWidget * container) {
    tabber = container;
    tabber -> setContextMenuPolicy(Qt::CustomContextMenu);

    connect(tabber, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));
    connect(tabber, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseRequested(int)));
    store = new DataStore("tabs.json");
    load();
}

Tabber::~Tabber() {
    delete store;
    delete tabber;
}

int Tabber::addTab(QString name, CBHash settings) {
    int index = tabber -> addTab(new Tab(settings, tabber), name);
    tabber -> setCurrentIndex(index);
    return index;
}

Tab * Tabber::currentTab() {
     return static_cast<Tab *>(tabber->currentWidget());
}

void Tabber::save() {
    Player::instance() ->stop();
    store -> clear();
    for(int i = 0; i < tabber->count(); i++) {
        Tab * tab = (Tab*)(tabber -> widget(i));
        store->write(QString::number(i) + "h", tab -> toJSON(tab -> getName()));
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
            new_tab -> updateHeader();
        }
    }
}
