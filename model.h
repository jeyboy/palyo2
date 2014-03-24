#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QFont>
#include <QBrush>

#include "model_item.h"
#include "icon_provider.h"
#include "library.h"


//#ifndef MILIST
//#define MILIST

//  #include <QList>
//  typedef QList<ModelItem> MIList;
//  Q_DECLARE_METATYPE(MIList);
//#endif // MILIST

class ModelItem;

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    TreeModel(QJsonObject * attrs = 0, QWidget *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(ModelItem * item) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void startRowInsertion(const QModelIndex &parent, int position = -1);
    void endRowInsertion();
    void appendRow(QString path, ModelItem * parent);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    void repaint();
    void refreshItem(ModelItem * item);

    ModelItem *getItem(const QModelIndex &index) const;
    ModelItem * root() const;

    ModelItem * buildPath(QString path);
    ModelItem * addFolder(QString folder_name, ModelItem * parent);
    int count;

    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData * mimeData(const QModelIndexList &indexes) const;

private:
    ModelItem *rootItem;
};


#endif // MODEL_H
