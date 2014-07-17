#ifndef WIDGETS_H
#define WIDGETS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QBoxLayout>
#include <QDateTime>

#include "misc/data_store.h"

#include "model/view_types.h"

#include "model/list/list_view.h"
#include "model/tree/tree_view.h"
#include "model/level_tree/level_tree_view.h"
#include "model/level_tree_breadcrumb/level_tree_breadcrumb_view.h"
#include "model/web/vk/vk_view.h"
#include "model/web/soundcloud/soundcloud_view.h"

#include "model/model.h"
#include "model/model_item.h"

#include "override/widget.h"

#ifndef CBHASH
#define CBHASH
  #include <QHash>
  typedef QHash <QString, int> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

class Widgets : public QObject {
    Q_OBJECT
public:
    ~Widgets() {
        delete store;
    }

    static Widgets * instance(QObject * parent);
    static void close() {
        delete self;
    }

    Widget * toActiveWidget();
    Widget * currentWidget();
    Widget * commonWidget();
    void updateIconSize();
    Widget * addWidget(QString name, CBHash settings);

    void load(QMainWindow * window);
    void save(QMainWindow * window);

//    QDockWidget * createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place = Qt::RightDockWidgetArea);

private:
    Widgets(QObject * parent) : QObject(parent) {
        commonPlaylist = 0;
        activePlaylist = 0;

        store = new DataStore("tabs.json");
    }

    static Widgets *self;
    Widget * commonPlaylist, * activePlaylist;
    DataStore * store;

};

#endif // WIDGETS_H
