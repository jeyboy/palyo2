#include "vk_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QJsonObject * hash, QObject *parent) : TreeModel(hash, parent) {
    connect(VkApi::instance(), SIGNAL(audioListReceived(QJsonObject &)), this, SLOT(proceedAudioList(QJsonObject &)));

    if (hash != 0) {
        QJsonObject res = hash -> value("vk").toObject();
        VkApi::instance() -> setParams(res.value("t").toString(), res.value("u").toString(), res.value("e").toString(), res.value("i").toString());

        if (VkApi::instance() -> isRefreshRequire())
            VkApi::instance() -> getUserAudioList();
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


//TODO: extend folder creation for new params
//TODO: create separate items for vk ?

void VkModel::proceedAudioList(QJsonObject & hash) {
    qDebug() << hash.keys();
    QJsonArray filesAr, ar = hash.value("albums").toArray();
//    folder_id: curr, title: curr.title, items: API.audio.get({album_id: curr}).items
    QJsonObject iterObj, fileIterObj;
    WebFileItem * newItem;

    qDebug() << ar;

    if (ar.count() > 0) {
        ModelItem * folder;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();

            folder = addFolder(iterObj.value("title").toString(), rootItem, QString::number(iterObj.value("folder_id").toInt()));

            filesAr = iterObj.value("items").toArray();

//            qDebug() << filesAr;

            foreach(QJsonValue obj, filesAr) {
                fileIterObj = obj.toObject();
                newItem = new WebFileItem(
                            fileIterObj.value("url").toString(),
                            fileIterObj.value("artist").toString() + " - " + fileIterObj.value("title").toString(),
                            fileIterObj.value("id").toString(),
                            folder,
                            fileIterObj.value("genre_id").toInt(-1),
                            fileIterObj.value("duration").toInt(-1)
                            );

                appendRow(newItem -> toModelItem());
            }
        }
    }

    ar = hash.value("audio_list").toObject().value("items").toArray();

//    qDebug() << ar;

    if (ar.count() > 0) {
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
