#include "model/tree/tree_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {

}

TreeModel::~TreeModel() {
//    delete rootItem;
}
