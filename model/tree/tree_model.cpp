#include "model/tree/tree_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {

}

TreeModel::~TreeModel() {
//    delete rootItem;
}

ModelItem * TreeModel::buildPath(QString path) {
    QStringList list = path.split('/', QString::SkipEmptyParts);
    ModelItem * curr = rootItem;

    foreach(QString piece, list) {
        curr = addFolder(piece, curr);
    }

    return curr;
}
