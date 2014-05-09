#include "vk_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QJsonObject * hash, QObject *parent) : TreeModel(hash, parent) {
    connect(VkApi::instance(), SIGNAL(audioListReceived(QJsonObject &)), this, SLOT(proceedAudioList(QJsonObject &)));

    if (hash != 0) {
        QJsonObject res = hash -> value("vk").toObject();
        VkApi::instance() -> setParams(res.value("t").toString(), res.value("u").toString(), res.value("e").toString());
    } else {
        VkApi::instance() -> getUserAudioList();
    }
}

VkModel::~VkModel() {
//    delete rootItem;
}

//ModelItem * VkModel::buildPath(QString path) {
//    QStringList list = path.split('/', QString::SkipEmptyParts);
//    ModelItem * curr = rootItem;

//    foreach(QString piece, list) {
//        curr = addFolder(piece, curr);
//    }

//    return curr;
//}

void VkModel::proceedAudioList(QJsonObject & hash) {
    qDebug() << hash.keys();
    QJsonArray ar = hash.value("albums").toObject().value("items").toArray();
    QJsonObject iterObj;

    qDebug() << ar;

    if (ar.count() > 0) {
        ModelItem * folder;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            folder = addFolder(iterObj.value("title").toString(), rootItem);
        }
    }

    ar = hash.value("audio_list").toObject().value("items").toArray();

    qDebug() << ar;

    if (ar.count() > 0) {
        WebFileItem * newItem;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            newItem = new WebFileItem(
                        iterObj.value("url").toString(),
                        iterObj.value("artist").toString() + " - " + iterObj.value("title").toString(),
                        iterObj.value("id").toString(),
                        root(),
                        iterObj.value("genre_id").toInt(-1),
                        iterObj.value("duration").toInt(-1)
                        );

            appendRow(newItem -> toModelItem());
        }
    }

    refresh();
}
