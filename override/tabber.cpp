#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabber.h"
#include <QApplication>
#include <QMessageBox>

//TODO:// add tab position for each tab
//    tabber->setTabPosition(TabPosition);


//tabber -> tabBar() -> setTabIcon();

void Tabber::setNoTabsStyle() {
//    tabber -> setStyleSheet(
//        "QTabWidget::pane {"
//            "border-top: 1px solid #C2C7CB;"
//            "background-image: url(:/tab_back);"
//            "background-position: center center;"
//            "background-repeat: no-repeat;"
//        "}"
//    );
}

void Tabber::updateActiveTab(QWidget * last, QWidget * current) {
    int index;

    if (last != 0) {
        index = tabber -> indexOf(last -> parentWidget());
        tabber -> setTabIcon(index, QIcon());
    }

    if (current != 0) {
        index = tabber -> indexOf(current -> parentWidget());
        tabber -> setTabIcon(index, QIcon(":/active_tab"));
    }
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

    if (del_tab == commonPlaylist)
        commonPlaylist = 0;

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
    connect(Player::instance(), SIGNAL(playlistChanged(QWidget *, QWidget *)), this, SLOT(updateActiveTab(QWidget *, QWidget *)));

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

Tab * Tabber::toActiveTab() {
    if (Player::instance() -> currentPlaylist()) {
        Tab * tab = (Tab *)Player::instance() -> currentPlaylist() -> parentWidget();
        tabber -> setCurrentIndex(tabber -> indexOf(tab));
        return tab;
    } else return 0;
}

Tab * Tabber::currentTab() {
    return static_cast<Tab *>(tabber -> currentWidget());
}

Tab * Tabber::commonTab() {
    if (commonPlaylist == 0) {
        TabDialog dialog;
        CBHash settings = dialog.getSettings(); // get default settings
        settings.insert("p", 1); // is playlist
        settings.insert("t", 2); // is one level tree
        settings.insert("c", 1); // is common

        addTab("Common", settings);
        commonPlaylist = currentTab();
    } else {
        tabber -> setCurrentIndex(tabber -> indexOf(commonPlaylist));
    }

    return commonPlaylist;
}

void Tabber::save() {
    Player::instance() -> pause();
    store -> clear();
    Tab * tab;
    for(int i = 0; i < tabber -> count(); i++) {
        tab = (Tab*)(tabber -> widget(i));
        if (tab == commonPlaylist) {
            // logic for common playlist // at this time common list did not save
        } else {
            store -> write(QString::number(i) + "h", tab -> toJSON(tab -> getName()));
        }
    }

    store -> save();
    Player::instance() -> stop();
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

            if (tab.contains("pv")) {
                new_tab -> getList() -> execItem(new_tab -> getList() -> fromPath(tab.value("pp").toString()), true);
                Player::instance() -> setPosition(tab.value("pt").toInt());
                qDebug() << "HUDO " << tab.value("pt").toInt();
//                Player::instance() -> play();
            }
        }
    }
}

void Tabber::updateIconSize(bool isBigIcon) {
    Tab * tab;
    int dimension = isBigIcon ? 32 : 16;
    QSize size(dimension, dimension);
    for(int i = 0; i < tabber -> count(); i++) {
        tab = (Tab*)(tabber -> widget(i));
        tab -> getList() -> setIconSize(size);
    }
}
