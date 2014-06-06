#include "vk_model.h"
#include "media/library.h"
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
    connect(VkApi::instance(), SIGNAL(errorReceived(QJsonObject &)), this, SLOT(errorReceived(QJsonObject &)));
}

VkModel::~VkModel() {

}

QString VkModel::getTabUid() const {
    return tabUid;
}

//TODO: update only links
void VkModel::refresh() {
    emit showSpinner();
//    clearAll();
//    Library::instance() -> clearRemote();
//    VkApi::instance() -> clearData();
    QApplication::processEvents();

    VkApi::instance() -> getUserAudioList(FuncContainer(this, SLOT(proceedAudioList(QJsonObject &))), tabUid);
    QApplication::processEvents();
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
    QHash<ModelItem*, QString> store;
    rootItem -> accumulateUids(store);

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

                proceedAudioList(filesAr, folder, store);
            }
        }
    }

/////////////////////////////////////////////////////////////////////
    ar = hash.value("audio_list").toObject().value("items").toArray();

    qDebug() << ar;

    if (ar.count() > 0) {        
        proceedAudioList(ar, root(), store);
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

    qDebug() << "STORE LENGTH: " << store.count();
    foreach(ModelItem * item, store.keys()) {
        QModelIndex ind = index(item);
        removeRow(ind.row(), ind.parent());
    }

    TreeModel::refresh();
    emit hideSpinner();
}

void VkModel::proceedAudioList(QJsonArray & ar, ModelItem * parent, QHash<ModelItem*, QString> & store) {
    QJsonObject fileIterObj;
    VkFile * newItem;
    QString id, owner;
    QList<ModelItem *> items;

    foreach(QJsonValue obj, ar) {
        fileIterObj = obj.toObject();
        owner = QString::number(fileIterObj.value("owner_id").toInt());
        id = QString::number(fileIterObj.value("id").toInt());
        items = store.keys(ModelItem::buildUid(owner, id));
        if (items.isEmpty()) {
            newItem = new VkFile(
                        fileIterObj.value("url").toString(),
                        fileIterObj.value("artist").toString() + " - " + fileIterObj.value("title").toString(),
                        owner,
                        id,
                        parent,
                        fileIterObj.value("genre_id").toInt(-1),
                        Duration::fromSeconds(fileIterObj.value("duration").toInt(0))
                        );

            appendRow(newItem -> toModelItem());
            qDebug() << "NEW ITEM " << newItem -> data(0);
        } else {
            foreach(ModelItem * item, items) {
//                store.remove(item);
                item -> setPath(fileIterObj.value("url").toString());
                item -> setGenre(fileIterObj.value("genre_id").toInt(-1));
            }
            store.remove(items.first());
        }
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

void VkModel::errorReceived(QJsonObject & obj) {
    qDebug() << "!!!!!!!!!!! Some shit happened :( " << obj;
    emit hideSpinner();
}
