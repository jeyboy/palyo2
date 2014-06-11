#include "soundcloud_model.h"
#include "media/library.h"
#include "media/player.h"
#include "misc/func_container.h"
#include <QDebug>

///////////////////////////////////////////////////////////

SoundcloudModel::SoundcloudModel(QString uid, QJsonObject * hash, QObject *parent) : TreeModel(hash, parent) {
    tabUid = uid;

    if (hash == 0) {
        SoundcloudApi::instance() -> getUidInfo(FuncContainer(this, SLOT(proceedResponse(QJsonObject &))), tabUid);
    }

//    connect(Player::instance(), SIGNAL(remoteUnprocessed()), this, SLOT(refresh()));
}

SoundcloudModel::~SoundcloudModel() { }

QString SoundcloudModel::getTabUid() const { return tabUid; }

void SoundcloudModel::refresh() {
    TreeModel::refresh();
//    emit showSpinner();
//    QApplication::processEvents();
//    SoundcloudApi::instance() -> getUidInfo(FuncContainer(this, SLOT(proceedAudioList(QJsonObject &))), tabUid);
//    QApplication::processEvents();
}

void SoundcloudModel::proceedResponse(QJsonObject & hash) {
    QHash<ModelItem*, QString> store;
    rootItem -> accumulateUids(store);

    QJsonArray filesAr, ar;
    QJsonObject iterObj;

//    QJsonArray filesAr, ar = hash.value("albums").toArray();
//    QJsonObject iterObj;

//    qDebug() << ar;

//    if (ar.count() > 0) {
//        ModelItem * folder;

//        foreach(QJsonValue obj, ar) {
//            iterObj = obj.toObject();

//            filesAr = iterObj.value("items").toArray();

//            if (filesAr.size() > 0) {
//                folder = addFolder(iterObj.value("title").toString(), rootItem, QString::number(iterObj.value("folder_id").toInt()));

//                proceedAudioList(filesAr, folder, store);
//            }
//        }
//    }

/////////////////////////////////////////////////////////////////////
    ar = hash.value("audio_list").toArray();

    qDebug() << ar;

    if (ar.count() > 0) {
        proceedResponse(ar, root(), store);
    }
/////////////////////////////////////////////////////////////////////
//    ar = hash.value("groups").toArray();

//    if (ar.count() > 0) {
//        foreach(QJsonValue obj, ar) {
//            iterObj = obj.toObject();
//            VkApi::instance() -> addGroup(
//                            QString::number(iterObj.value("id").toInt()),
//                            iterObj.value("title").toString()
//                        );
//        }
//    }
/////////////////////////////////////////////////////////////////////
//    ar = hash.value("friends").toArray();

//    if (ar.count() > 0) {
//        foreach(QJsonValue obj, ar) {
//            iterObj = obj.toObject();
//            VkApi::instance() -> addFriend(
//                            QString::number(iterObj.value("id").toInt()),
//                            iterObj.value("title").toString()
//                        );
//        }
//    }

    qDebug() << "STORE LENGTH: " << store.count();
    foreach(ModelItem * item, store.keys()) {
        QModelIndex ind = index(item);
        removeRow(ind.row(), ind.parent());
    }

    TreeModel::refresh();
    emit hideSpinner();

    if (count == 0)
        emit showMessage(QString("This object did not have any items. Use wall parse from context menu"));
}

void SoundcloudModel::proceedResponse(QJsonArray & ar, ModelItem * parent, QHash<ModelItem*, QString> & store) {
    QJsonObject fileIterObj;
    SoundcloudFile * newItem;
    QString id, owner, url;
    QList<ModelItem *> items;
    bool original;

    foreach(QJsonValue obj, ar) {
        fileIterObj = obj.toObject();

        if (fileIterObj.isEmpty()) continue;

        owner = QString::number(fileIterObj.value("user_id").toInt());
        id = QString::number(fileIterObj.value("id").toInt());
        items = store.keys(ModelItem::buildUid(owner, id));
        if (items.isEmpty()) {
            url = fileIterObj.value("download_url").toString();
            if (url.isEmpty()) {
                url = fileIterObj.value("stream_url").toString();
                original = false;
            } else { original = true;}
            if (url.isEmpty()) continue;

//            video_url
//            bpm

            newItem = new SoundcloudFile(
                            url,
                            fileIterObj.value("title").toString(),
                            original ? fileIterObj.value("original_format").toString() : "mp3",
                            owner,
                            id,
                            parent,
                            Genre::instance() -> toInt(fileIterObj.value("genre").toString()),
                            Duration::fromMillis(fileIterObj.value("duration").toInt(0)),
                            original ? fileIterObj.value("original_content_size").toInt() : -1
                        );

            appendRow(newItem -> toModelItem());
            qDebug() << "NEW ITEM " << original << " " << newItem -> data(0);
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

void SoundcloudModel::errorReceived(int code, QString & msg) {
    emit showMessage("!!!!!!!!!!! Some shit happened :( " + msg);
    emit hideSpinner();
}
