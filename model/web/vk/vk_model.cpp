#include "vk_model.h"
#include "misc/func_container.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QString uid, QJsonObject * hash, QObject *parent) : TreeModel(hash, parent) {
    qDebug() << "VK UID " << uid;
    tabUid = uid;

    if (hash == 0) {
        VkApi::instance() -> getUserAudioList(FuncContainer(this, SLOT(proceedAudioList(QJsonObject &))), tabUid);
    }

    connect(IpChecker::instance(), SIGNAL(ipChanged()), this, SLOT(refresh()));
}

VkModel::~VkModel() {

}

QString VkModel::getTabUid() const {
    return tabUid;
}

//TODO: update only links
void VkModel::refresh() {
    emit showSpinner();
    clearAll();
//    VkApi::instance() -> clearData();
    QApplication::processEvents();

    VkApi::instance() -> getUserAudioList(FuncContainer(this, SLOT(proceedAudioList(QJsonObject &))), tabUid);
    QApplication::processEvents();
    emit hideSpinner();
}


//void VkModel::refresh() {
//    emit showSpinner();
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Update !!!!!!!!!!!!!!!!!!!!";
////    clearAll();
////    VkApi::instance() -> clearData();
//    QApplication::processEvents();

//    QHash<ModelItem*, QString> store;
//    rootItem -> accumulateUids(store);

//    VkApi::instance() -> refreshAudioList(
//                FuncContainer(this, SLOT(proceedAudioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &))),
//                store
//                );
//    QApplication::processEvents();
//    emit hideSpinner();
//}


void VkModel::proceedAudioList(QJsonObject & hash) {
    QJsonArray filesAr, ar = hash.value("albums").toArray();
    QJsonObject iterObj;

    qDebug() << ar;

    if (ar.count() > 0) {
        ModelItem * folder;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();

            filesAr = iterObj.value("items").toArray();

            if (filesAr.size() > 0) {
                folder = addFolder(iterObj.value("title").toString(), rootItem, QString::number(iterObj.value("folder_id").toInt()));

                proceedAudioList(filesAr, folder);
            }
        }
    }

/////////////////////////////////////////////////////////////////////
    ar = hash.value("audio_list").toObject().value("items").toArray();

    qDebug() << ar;

    if (ar.count() > 0) {        
        proceedAudioList(ar, root());
    }
/////////////////////////////////////////////////////////////////////
    ar = hash.value("groups").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            VkApi::instance() -> addGroup(
                            QString::number(iterObj.value("id").toInt()),
                            iterObj.value("title").toString()
                        );
        }
    }
/////////////////////////////////////////////////////////////////////
    ar = hash.value("friends").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            VkApi::instance() -> addFriend(
                            QString::number(iterObj.value("id").toInt()),
                            iterObj.value("title").toString()
                        );
        }
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

void VkModel::proceedAudioListUpdate(QJsonObject & obj, QHash<ModelItem *, QString> & collation) {
    QJsonObject iterObj;
    QJsonArray ar = obj.value("response").toArray();
    QString uid;
    ModelItem * item;

    foreach(QJsonValue obj, ar) {
        iterObj = obj.toObject();
        uid = QString::number(iterObj.value("owner_id").toInt()) + "_" + QString::number(iterObj.value("id").toInt());

        item = collation.key(uid);
        item -> setPath(iterObj.value("url").toString());
    }
}
