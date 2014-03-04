#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QBoxLayout>
#include <QJsonObject>

#include "itemlist.h"
#include "model.h"
#include "model_item.h"


#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <char, bool> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

class ItemList;

class Tab : public QWidget {
    Q_OBJECT
public:
    void init(QJsonObject * attrs = 0);

    explicit Tab(CBHash params, QWidget * parent = 0);
    explicit Tab(QJsonObject json_attrs, QWidget * parent = 0);
    ~Tab();

    void updateHeader(QTabWidget * parent = 0);
    QJsonObject toJSON(QString name);

    bool isRemoveFileWithItem();

private:
    ItemList * list;
    CBHash settings;

//private slots:
//    void handleListClicked(const QModelIndex &index);
};

#endif // TAB_H

//QStandardItemModel *model = new QStandardItemModel(3, 3);

//QModelIndex index = model->index(1, 0, QModelIndex());

//model->setHeaderData( 0, Qt::Horizontal, "numéro" );
//model->setHeaderData( 1, Qt::Horizontal, "prénom" );
//model->setHeaderData( 2, Qt::Horizontal, "nom" );

//model->setData( index, "1", Qt::DisplayRole );

//listView->setModel(model);






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

