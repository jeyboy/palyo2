#include "vk_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QJsonObject * hash, QObject *parent) : TreeModel(hash, parent) {
    connect(VkApi::instance(), SIGNAL(audioListReceived(QJsonObject &)), this, SLOT(proceedAudioList(QJsonObject &)));

    if (hash == 0) {
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
    QJsonArray ar = hash.value("folders").toArray();
    QJsonObject iterObj;
    ModelItem * folder;

    qDebug() << ar;

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            folder = addFolder(iterObj.value("name").toString(), rootItem);
            qDebug() << folder -> fullPath();
        }
    }

    ar = hash.value("audio_list").toArray();

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
}
