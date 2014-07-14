#include "widgets.h"
#include <qDebug>

Widgets *Widgets::self = 0;

Widgets *Widgets::instance(QObject * parent) {
    if(!self)
        self = new Widgets(parent);
    return self;
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
    qDebug() << dock -> titleBarWidget();
    dock -> setWindowFlags(Qt::WindowMinMaxButtonsHint);
    dock -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom, ((QWidget *)parent())));
//    dock -> showFullScreen();
    dock -> setWidget(content);
    dock -> layout() -> setAlignment(content, Qt::AlignCenter);
    ((QMainWindow *)parent()) -> addDockWidget(place, dock);
    //    viewMenu->addAction(dock->toggleViewAction());
    return dock;
//    ((QWidget *)parent())->tabifyDockWidget(dockWidget1,dockWidget2);
}
