#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QBoxLayout>
#include <QJsonObject>

#include "model/view_types.h"

#include "model/list/list_view.h"
#include "model/tree/tree_view.h"
#include "model/level_tree/level_tree_view.h"
#include "model/level_tree_breadcrumb/level_tree_breadcrumb_view.h"
#include "model/web/vk/vk_view.h"
#include "model/web/soundcloud/soundcloud_view.h"

#include "model/model.h"
#include "model/model_item.h"

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, int> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

class Tab : public QWidget {
    Q_OBJECT
public:
    void init(CBHash params, QJsonObject * hash = 0);

    explicit Tab(CBHash params, QWidget * parent = 0);
    explicit Tab(QJsonObject hash, QWidget * parent = 0);
    ~Tab();

    QString getName() const;
    void setName(QString newName);

    View * getView() const;
    QJsonObject toJSON(QString name);

    bool isEditable() const;

public slots:
    void updateHeader(int new_count = -1);

protected slots:
    void startRoutine();
    void stopRoutine();

protected:
    void setNameWithCount(QString name);

private:
    View * view;
    QTabWidget * tabber;

    QLabel * spinnerContainer;

//private slots:
//    void handleListClicked(const QModelIndex &index);
};

#endif // TAB_H

//#include <QListView>
//#include <QStandardItemModel>
//#include <QList>
//#include <QColor>
//#include <QFont>

//QStandardItemModel *model = new QStandardItemModel();

//QList<QString> colors;
//colors << "red" << "blue" << "cyan" << "magenta" << "black" << "yellow";
//QFont txtFont("Arial", 28);

//foreach (const QString &clrName, colors)
//{
//    QStandardItem *colorItem = new QStandardItem(clrName);

//// The Qt::BackgroundRole takes a brush and paints the background with it
////        colorItem->setData(QBrush(QColor(clrName)), Qt::BackgroundRole);

//// The Qt::DecorationRole takes a color or a pixmap, and draws it in a small icon
//// to the left of the icon
//    colorItem->setData(QColor(clrName), Qt::DecorationRole);

//// Qt::FontRole determines the font of the item
//    colorItem->setData(txtFont, Qt::FontRole);

//    colorItem->setData(QBrush(QColor(Qt::white).darker(160)),
//                       Qt::ForegroundRole);

//    model->appendRow(colorItem);
//}



//tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->hide();

//QStandardItem *item=new QStandardItem();
//item->setData(color,Qt::BackgroundColorRole);



//QListWidget t;
//t.addItem("first");
//t.addItem("second");
//t.item(0)->setForeground(Qt::red);
//t.item(1)->setForeground(Qt::blue);



//QTreeView *treeView = new QTreeView;

//MyItemModel *model = new MyItemModel(this);

//treeView->setModel(model);



//MyItemModel *sourceModel = new MyItemModel(this);
//QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);

//proxyModel->setSourceModel(sourceModel);
//treeView->setModel(proxyModel);



//treeView->setSortingEnabled(true);



//proxyModel->sort(2, Qt::AscendingOrder);

//proxyModel->setFilterRegExp(QRegExp(".png", Qt::CaseInsensitive,
//                                    QRegExp::FixedString));
//proxyModel->setFilterKeyColumn(1);

