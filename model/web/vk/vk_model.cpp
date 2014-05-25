#include "vk_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QJsonObject * hash, QObject *parent) : TreeModel(hash, parent) {
    connect(VkApi::instance(), SIGNAL(audioListReceived(QJsonObject &)), this, SLOT(proceedAudioList(QJsonObject &)));

    if (hash != 0) {
        QJsonObject res = hash -> value("vk").toObject();
        VkApi::instance(res.value("t").toString(), res.value("u").toString(), res.value("e").toString());

//        if (VkApi::instance() -> isRefreshRequire()) {
//            clearAll();
//            VkApi::instance() -> getUserAudioList();
//        }

    } else {
        VkApi::instance() -> getUserAudioList();
    }

    connect(IpChecker::instance(), SIGNAL(ipChanged()), this, SLOT(refresh()));
}

VkModel::~VkModel() {

}

//TODO: update only links
void VkModel::refresh() {
    emit showSpinner();
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Update !!!!!!!!!!!!!!!!!!!!";
    clearAll();
    QApplication::processEvents();
    VkApi::instance() -> getUserAudioList();
    QApplication::processEvents();
    emit hideSpinner();
}


void VkModel::proceedAudioList(QJsonObject & hash) {
    qDebug() << hash.keys();
    QJsonArray filesAr, ar = hash.value("albums").toArray();
//    folder_id: curr, title: curr.title, items: API.audio.get({album_id: curr}).items
    QJsonObject iterObj;

    qDebug() << ar;

    if (ar.count() > 0) {
        ModelItem * folder;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();

            folder = addFolder(iterObj.value("title").toString(), rootItem, QString::number(iterObj.value("folder_id").toInt()));
            filesAr = iterObj.value("items").toArray();

            proceedAudioList(filesAr, folder);
        }
    }

    ar = hash.value("audio_list").toObject().value("items").toArray();

//    qDebug() << ar;

    if (ar.count() > 0) {        
        proceedAudioList(ar, root());
    }

    TreeModel::refresh();
}

void VkModel::proceedAudioList(QJsonArray & ar, ModelItem * parent) {
    QJsonObject fileIterObj;
    VkFile * newItem;

    foreach(QJsonValue obj, ar) {
        fileIterObj = obj.toObject();
        newItem = new VkFile(
                    fileIterObj.value("url").toString(),
                    fileIterObj.value("artist").toString() + " - " + fileIterObj.value("title").toString(),
                    QString::number(fileIterObj.value("owner_id").toInt()),
                    QString::number(fileIterObj.value("id").toInt()),
                    parent,
                    fileIterObj.value("genre_id").toInt(-1),
                    Duration::fromSeconds(fileIterObj.value("duration").toInt(0))
                    );

        appendRow(newItem -> toModelItem());
    }
}


