#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabber.h"
#include <QApplication>
#include <QMessageBox>

//TODO:// add tab position for each tab
//    tabber->setTabPosition(TabPosition);


//tabber -> tabBar() -> setTabIcon();

void Tabber::setNoTabsStyle() {
    tabber -> setStyleSheet(
        "QTabWidget::pane {"
            "border-top: 1px solid #C2C7CB;"
            "background-image: url(:/tab_back);"
            "background-position: center center;"
            "background-repeat: no-repeat;"
        "}"
    );
}

void Tabber::handleCurrentChanged(int index) {
    if (index == -1) {
        Player::instance() -> setActivePlaylist(0);
    } else {
        Tab * new_tab = static_cast<Tab *>(tabber -> widget(index));

        Player::instance() -> setActivePlaylist(const_cast<View *>(new_tab -> getList()));
    }
}

void Tabber::handleTabCloseRequested(int index) {
    Tab * del_tab = static_cast<Tab *>(tabber -> widget(index));
    if (Player::instance() -> currentPlaylist() == del_tab -> getList()) {
        Player::instance() -> removePlaylist();
    }

    if (Player::instance() -> currentActivePlaylist() == del_tab -> getList()) {
        Player::instance() -> setActivePlaylist(0);
    }

    tabber -> removeTab(index);

    if (tabber -> count() == 0) {
        setNoTabsStyle();
    }
}

Tabber::Tabber(QTabWidget * container) {
    tabber = container;
    commonPlaylist = 0;

    setNoTabsStyle();
//    if (tabber -> count() == 0) {
//        setNoTabsStyle();
//    }

    connect(tabber, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseRequested(int)));
    connect(tabber, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));

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
    tabber -> setStyleSheet("");
    return index;
}

Tab * Tabber::currentTab() {
    return static_cast<Tab *>(tabber -> currentWidget());
}

Tab * Tabber::commonTab() {
    if (commonPlaylist == 0) {
        TabDialog dialog;
        addTab("Common", dialog.getSettings()); // set default settings
        commonPlaylist = currentTab();
    } else {
        tabber -> setCurrentIndex(tabber -> indexOf(commonPlaylist));
    }

    return commonPlaylist;
}

void Tabber::save() {
    Player::instance() -> stop();
    store -> clear();
    for(int i = 0; i < tabber -> count(); i++) {
        Tab * tab = (Tab*)(tabber -> widget(i));
        if (tab == commonPlaylist) {
            // logic for common playlist // at this time common list did not save
        } else {
            store -> write(QString::number(i) + "h", tab -> toJSON(tab -> getName()));
        }
    }

    store -> save();
}

void Tabber::load() {
    if (store -> state) {
        QJsonObject tab;

        foreach(QString key, store -> keys()) {
            tab = store -> read(key).toObject();
            Tab * new_tab = new Tab(tab, tabber);

            if (new_tab -> getList() -> isCommon())
                commonPlaylist = new_tab;

            tabber -> addTab(new_tab, tab["n"].toString());
            new_tab -> updateHeader();
        }
    }
}
