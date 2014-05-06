#include "vk_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {
    if (hash == 0) {
        VkApi::getUserAudios()
    }
}

VkModel::~VkModel() {
//    delete rootItem;
}

ModelItem * VkModel::buildPath(QString path) {
    QStringList list = path.split('/', QString::SkipEmptyParts);
    ModelItem * curr = rootItem;

    foreach(QString piece, list) {
        curr = addFolder(piece, curr);
    }

    return curr;
}
