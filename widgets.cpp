#include "widgets.h"
#include "titlebar.h"
#include <qDebug>

Widgets *Widgets::self = 0;

Widgets *Widgets::instance(QObject * parent) {
    if(!self)
        self = new Widgets(parent);
    return self;
}













Tabber::Tabber(QWidget *parent) : QTabWidget(parent) {
    commonPlaylist = 0;

//    tabber -> setTabBar(0); // TODO: custom tab bar
    setTabPosition((QTabWidget::TabPosition)Settings::instance() -> getTabPosition());
    setUsesScrollButtons(Settings::instance() -> getScrollButtonUsage());

    setStyleSheet(Stylesheets::treeViewStyles());

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
    connect(Player::instance(), SIGNAL(playlistChanged(QWidget *, QWidget *)), this, SLOT(updateActiveTab(QWidget *, QWidget *)));

    store = new DataStore("tabs.json");
}

Tabber::~Tabber() {
    delete store;
}

int Tabber::addTab(QString name, CBHash settings) {
    int index = QTabWidget::addTab(new Tab(settings, this), name);
    (static_cast<Tab *>(widget(index))) -> updateHeader();
    setCurrentIndex(index);
    return index;
}

Tab * Tabber::toActiveTab() {
    if (Player::instance() -> currentPlaylist()) {
        Tab * tab = (Tab *)Player::instance() -> currentPlaylist() -> parentWidget();
        setCurrentIndex(indexOf(tab));
        return tab;
    } else return 0;
}

Tab * Tabber::currentTab() {
    return static_cast<Tab *>(currentWidget());
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
        setCurrentIndex(indexOf(commonPlaylist));
    }

    return commonPlaylist;
}

void Tabber::load() {
    if (store -> state) {
        QJsonObject tab;

        foreach(QString key, store -> keys()) {
            tab = store -> read(key).toObject();
            Tab * new_tab = new Tab(tab, this);

            if (new_tab -> getView() -> isCommon())
                commonPlaylist = new_tab;

            QTabWidget::addTab(new_tab, tab["n"].toString());
            new_tab -> updateHeader();

            if (tab.contains("pv")) {
                new_tab -> getView() -> execItem(new_tab -> getView() -> fromPath(tab.value("pp").toString()), true);
                if (tab.contains("pt")) {
                    Player::instance() -> setStartPosition(tab.value("pt").toInt());
//                  Player::instance() -> play();
                }
            }
        }
    }
}

void Tabber::save() {
    Player::instance() -> pause();
    store -> clear();
    Tab * tab;
    for(int i = 0; i < count(); i++) {
        tab = (Tab*)(widget(i));
        if (tab == commonPlaylist) {
            // logic for common playlist // at this time common list did not save
        } else {
            store -> write(QString::number(i) + "h", tab -> toJSON(tab -> getName()));
        }
    }

    store -> save();
    Player::instance() -> stop();
}



void Tabber::updateIconSize() {
    Tab * tab;
    int dimension = Settings::instance() -> getIconHeight();
    QSize size(dimension, dimension);
    for(int i = 0; i < count(); i++) {
        tab = (Tab*)(widget(i));
        tab -> getView() -> setIconSize(size);
    }
}

void Tabber::updateActiveTab(QWidget * last, QWidget * current) {
    int index;

    if (last != 0) {
        index = indexOf(last -> parentWidget());
        setTabIcon(index, QIcon());
    }

    if (current != 0) {
        index = indexOf(current -> parentWidget());
        setTabIcon(index, QIcon(":/active_tab"));
    }
}

void Tabber::handleCurrentChanged(int index) {
    if (index == -1) {
        Player::instance() -> setActivePlaylist(0);
    } else {
        Tab * new_tab = static_cast<Tab *>(widget(index));

        Player::instance() -> setActivePlaylist(const_cast<View *>(new_tab -> getView()));
    }
}

void Tabber::handleTabCloseRequested(int index) {
    Tab * del_tab = static_cast<Tab *>(widget(index));

    if (del_tab -> getView() -> getModel() -> itemsCount() > 0)
        if (QMessageBox::warning(this, "Tab deletion", "Tab is not empty. Are you sure?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok)
            return;

    if (del_tab == commonPlaylist)
        commonPlaylist = 0;

    if (Player::instance() -> currentPlaylist() == del_tab -> getView()) {
        Player::instance() -> removePlaylist();
    }

    if (Player::instance() -> currentActivePlaylist() == del_tab -> getView()) {
        Player::instance() -> setActivePlaylist(0);
    }

    removeTab(index);
}


















void Widgets::load(QMainWindow * window, QJsonArray & bars) {
//    if (bars.count() > 0) {
//        QList<QString> barsList;
//        barsList.append("Media"); barsList.append("Media+"); barsList.append("Media+Position"); barsList.append("Media+Time");
//        barsList.append("Media+Volume"); barsList.append("Controls"); barsList.append("Spectrum");

//        QJsonObject obj, actionObj;
//        QString barName;
//        QToolBar * curr_bar;

//        foreach(QJsonValue bar, bars) {
//            obj = bar.toObject();
//            barName = obj.value("title").toString();
//            barsList.removeOne(barName);
//            curr_bar = linkNameToToolbars(barName);
//            curr_bar -> setMovable(obj.value("movable").toBool());

//            window -> addToolBar((Qt::ToolBarArea)obj.value("area").toInt(), curr_bar);

//            if (obj.contains("actions")) {
//                QJsonArray actions = obj.value("actions").toArray();

//                foreach(QJsonValue act, actions) {
//                    actionObj = act.toObject();
//                    addPanelButton(actionObj.value("name").toString(), actionObj.value("path").toString(), curr_bar);
//                }
//            }
//        }

//        recreateToolbars(window, barsList);
//    } else {
//        createToolbars(window);
//    }
}

void Widgets::save(QMainWindow * window, DataStore * settings) {
//    QList<QToolBar *> toolbars = window -> findChildren<QToolBar *>();
//    qDebug() << toolbars.length();

//    if (toolbars.length() > 0) {
//        QJsonArray toolbar_array = QJsonArray();
//        QJsonObject curr_tab;
//        QList<QAction*> actions;
//        ToolbarButton* button;

//        foreach(QToolBar * bar, toolbars) {
//            curr_tab = QJsonObject();

//            curr_tab.insert("area", window -> toolBarArea(bar));
//            curr_tab.insert("title", bar -> windowTitle());
//            curr_tab.insert("movable", bar -> isMovable());

//            if (bar -> windowTitle() != "Media"
//                    && bar -> windowTitle() != "Media+"
//                    && bar -> windowTitle() != "Media+Position"
//                    && bar -> windowTitle() != "Media+Time"
//                    && bar -> windowTitle() != "Media+Volume"
//                    && bar -> windowTitle() != "Controls"
//                    && bar -> windowTitle() != "Spectrum"
//               ) {
//                actions = bar -> actions();
//                if (actions.length() > 0) {
//                    QJsonArray action_array = QJsonArray();
//                    QJsonObject curr_act;

//                    foreach(QAction * act, actions) {
//                        if (QString(act -> metaObject() -> className()) == "QWidgetAction") {
//                            curr_act = QJsonObject();
//                            button = (ToolbarButton*) bar -> widgetForAction(act);

//                            curr_act.insert("path", button -> path);
//                            curr_act.insert("name", button -> text());
//                        }
//                        action_array.append(curr_act);
//                    }

//                    if (action_array.count() > 0)
//                        curr_tab.insert("actions", action_array);
//                }
//            }

//            toolbar_array.append(curr_tab);

//    //        bar -> toolButtonStyle();
//        }

//        settings -> write("bars", toolbar_array);
//    }
}

QDockWidget * Widgets::createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place) {
    QDockWidget * dock = new QDockWidget(name, (QWidget *)parent());
    dock -> setObjectName("DW_" + name + QString::number(QDateTime::currentMSecsSinceEpoch()));
    dock -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom, dock));
    dock -> setFeatures(
                QDockWidget::DockWidgetClosable |
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetVerticalTitleBar
    );
//    dock -> setTitleBarWidget(new Titlebar(dock));
//    dock -> showFullScreen();
    dock -> setWidget(content);
//    dock -> layout() -> setAlignment(content, Qt::AlignCenter);
    ((QMainWindow *)parent()) -> addDockWidget(place, dock);
    //    viewMenu->addAction(dock->toggleViewAction());
    return dock;
}
