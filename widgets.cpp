#include "widgets.h"
#include "titlebar.h"
#include <qDebug>

Widgets *Widgets::self = 0;

Widgets *Widgets::instance(QObject * parent) {
    if(!self)
        self = new Widgets(parent);
    return self;
}

Widget * Widgets::toActiveWidget() {
    if (Player::instance() -> currentPlaylist()) {
        Widget * tab = (Widget *)Player::instance() -> currentPlaylist() -> parentWidget();
        tab -> raise();
        return tab;
    } else return 0;
}

Widget * Widgets::currentWidget() {
    return activePlaylist;
}

Widget * Widgets::commonWidget() {
    if (commonPlaylist == 0) {
        TabDialog dialog;
        CBHash settings = dialog.getSettings(); // get default settings
        settings.insert("p", 1); // is playlist
        settings.insert("t", 2); // is one level tree
        settings.insert("c", 1); // is common

        addWidget("Common", settings);
        commonPlaylist = currentWidget();
    } else {
        commonPlaylist -> raise();
    }

    return commonPlaylist;
}

void Widgets::updateIconSize() {
//    Tab * tab;
//    int dimension = Settings::instance() -> getIconHeight();
//    QSize size(dimension, dimension);
//    for(int i = 0; i < count(); i++) {
//        tab = (Tab*)(widget(i));
//        tab -> getView() -> setIconSize(size);
//    }
}






//Tabber::Tabber(QWidget *parent) : QTabWidget(parent) {
//    commonPlaylist = 0;

////    tabber -> setTabBar(0); // TODO: custom tab bar
//    setTabPosition((QTabWidget::TabPosition)Settings::instance() -> getTabPosition());
//    setUsesScrollButtons(Settings::instance() -> getScrollButtonUsage());

//    setStyleSheet(Stylesheets::treeViewStyles());

//    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseRequested(int)));
//    connect(this, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
//    connect(Player::instance(), SIGNAL(playlistChanged(QWidget *, QWidget *)), this, SLOT(updateActiveTab(QWidget *, QWidget *)));

//    store = new DataStore("tabs.json");
//}

Widget * Widgets::addWidget(QString name, CBHash settings) {
    Widget * widget = new Widget(settings, name, (QWidget *)parent());
    ((QMainWindow *)parent()) -> addDockWidget(
                Qt::LeftDockWidgetArea,
                widget
    );
//    int index = QTabWidget::addTab(new Tab(settings, this), name);


//    int index = QTabWidget::addTab(new Tab(settings, this), name);
//    (static_cast<Tab *>(widget(index))) -> updateHeader();
//    setCurrentIndex(index);
    return widget;
}










//void Tabber::updateActiveTab(QWidget * last, QWidget * current) {
//    int index;

//    if (last != 0) {
//        index = indexOf(last -> parentWidget());
//        setTabIcon(index, QIcon());
//    }

//    if (current != 0) {
//        index = indexOf(current -> parentWidget());
//        setTabIcon(index, QIcon(":/active_tab"));
//    }
//}

//void Tabber::handleCurrentChanged(int index) {
//    if (index == -1) {
//        Player::instance() -> setActivePlaylist(0);
//    } else {
//        Tab * new_tab = static_cast<Tab *>(widget(index));

//        Player::instance() -> setActivePlaylist(const_cast<View *>(new_tab -> getView()));
//    }
//}

//void Tabber::handleTabCloseRequested(int index) {
//    Tab * del_tab = static_cast<Tab *>(widget(index));

//    if (del_tab -> getView() -> getModel() -> itemsCount() > 0)
//        if (QMessageBox::warning(this, "Tab deletion", "Tab is not empty. Are you sure?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok)
//            return;

//    if (del_tab == commonPlaylist)
//        commonPlaylist = 0;

//    if (Player::instance() -> currentPlaylist() == del_tab -> getView()) {
//        Player::instance() -> removePlaylist();
//    }

//    if (Player::instance() -> currentActivePlaylist() == del_tab -> getView()) {
//        Player::instance() -> setActivePlaylist(0);
//    }

//    removeTab(index);
//}


















void Widgets::load(QMainWindow * window) {
    if (store -> state) {
        QJsonObject tab, settings;

        foreach(QString key, store -> keys()) {
            tab = store -> read(key).toObject();
            settings = tab.value("settings").toObject();
            Widget * new_tab = new Widget(tab, settings.value("title").toString(), (QWidget *)parent()); // name, (QWidget *)parent()
            new_tab -> setObjectName(settings.value("name").toString("DW_" + QString::number(QDateTime::currentMSecsSinceEpoch())));
            window -> addDockWidget((Qt::DockWidgetArea)settings.value("area").toInt(1), new_tab);

            if (new_tab -> getView() -> isCommon())
                commonPlaylist = new_tab;

//            QTabWidget::addTab(new_tab, tab["n"].toString());
//            new_tab -> updateHeader();

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

void Widgets::save(QMainWindow * window) {
    store -> clear();
    Player::instance() -> pause();

    QList<QDockWidget *> widgets = window -> findChildren<QDockWidget *>();

    if (widgets.length() > 0) {
        int i = 0;
        QJsonObject curr_widget, obj;

        foreach(QDockWidget * widget, widgets) {
            curr_widget = QJsonObject();

            curr_widget.insert("area", window -> dockWidgetArea(widget));
            curr_widget.insert("title", widget -> windowTitle());
            curr_widget.insert("name", widget -> objectName());
//            curr_widget.insert("movable", bar -> isMovable());


            /////////////////////////////////
            if (widget == commonPlaylist) {
                // logic for common playlist // at this time common list did not save
            } else {
                obj = ((Widget *)widget) -> toJSON(((Widget *)widget) -> getName());
                obj.insert("settings", curr_widget);
                store -> write(QString::number(++i) + "h", obj);
            }
            /////////////////////////////////
        }
    }

    store -> save();
    Player::instance() -> stop();
}

//QDockWidget * Widgets::createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place) {
//    QDockWidget * dock = new QDockWidget(name, (QWidget *)parent());
//    dock -> setObjectName("DW_" + name + QString::number(QDateTime::currentMSecsSinceEpoch()));
//    dock -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom, dock));
//    dock -> setFeatures(
//                QDockWidget::DockWidgetClosable |
//                QDockWidget::DockWidgetMovable |
//                QDockWidget::DockWidgetFloatable |
//                QDockWidget::DockWidgetVerticalTitleBar
//    );
////    dock -> setTitleBarWidget(new Titlebar(dock));
////    dock -> showFullScreen();
//    dock -> setWidget(content);
////    dock -> layout() -> setAlignment(content, Qt::AlignCenter);
//    ((QMainWindow *)parent()) -> addDockWidget(place, dock);
//    //    viewMenu->addAction(dock->toggleViewAction());
//    return dock;
//}
