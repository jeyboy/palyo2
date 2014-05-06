#ifndef TREE_MODEL_H
#define TREE_MODEL_H

//#include <QAbstractItemModel>
//#include <QModelIndex>
//#include <QFont>
//#include <QBrush>

#include "model/model.h"

class TreeModel : public Model {
    Q_OBJECT

public:
    TreeModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~TreeModel();
};


#endif // TREE_MODEL_H
