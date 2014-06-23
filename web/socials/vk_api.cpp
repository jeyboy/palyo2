#include "web/socials/vk_api.h"

VkApi *VkApi::self = 0;

VkApi *VkApi::instance() {
    if(!self)
        self = new VkApi();
    return self;
}

VkApi * VkApi::instance(QJsonObject obj) {
    if(!self)
        self = new VkApi(obj);
    else
        VkApi::instance() -> fromJson(obj);
    return self;
}

void VkApi::fromJson(QJsonObject hash) {
    TeuAuth::fromJson(hash);
    WebApi::fromJson(hash);
}
QJsonObject VkApi::toJson() {
    QJsonObject root;

    root = TeuAuth::toJson(root);
    root = WebApi::toJson(root);

    return root;
}

bool VkApi::isConnected() {
    return !getToken().isEmpty() && !getUserID().isEmpty();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
QString VkApi::authUrl() const {
    return VkApiPrivate::authUrl();
}

QString VkApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.fragment());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("access_token")) {
        setParams(
                  query.queryItemValue("access_token"),
                  query.queryItemValue("user_id"),
                  query.queryItemValue("expires_in")
                  );
        return "accept";
    }

    return "";
}

///////////////////////////////////////////////////////////
/// WALL
///////////////////////////////////////////////////////////

void VkApi::getWallAttachmentsList(FuncContainer responseSlot, QString uid, int offset, int count) {
    uid = uid == "0" ? getUserID() : uid;
    QUrl url = VkApiPrivate::wallUrl(uid, getToken(), offset, count);

    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    responses.insert(m_http, responseSlot);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(wallResponse()));
}

void VkApi::wallResponse() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();

    QByteArray ar = reply -> readAll();
    QJsonObject doc = responseToJson(ar);

    FuncContainer func = responses.take(reply);

    if (doc.contains("error")) {
        qDebug() << ar;
        errorSend(doc, func.obj);
    } else {
        doc = doc.value("response").toObject();
        connect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
        emit audioListReceived(doc);
        disconnect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
    }

    reply -> close();
    delete reply;
}

///////////////////////////////////////////////////////////
/// AUDIO LIST
///////////////////////////////////////////////////////////

//TODO: has some troubles with ids amount in request
void VkApi::refreshAudioList(FuncContainer responseSlot, QHash<ModelItem *, QString> uids) {
    QUrl url = VkApiPrivate::audioRefreshUrl(QStringList(uids.values()), getToken());
    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    responses.insert(m_http, responseSlot);
    collations.insert(m_http, uids);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListResponse()));
}

void VkApi::getAudioList(FuncContainer responseSlot, QString uid) {
    uid = uid == "0" ? getUserID() : uid;
    QUrl url = VkApiPrivate::audioInfoUrl(uid, getUserID(), getToken());
    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    responses.insert(m_http, responseSlot);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListResponse()));
}

//void VkApi::getAudioAlbums(FuncContainer responseSlot, QString uid, int offset) {
//    uid = uid == "0" ? getUserID() : uid;
//    QUrl url = VkApiPrivate::audioAlbumsUrl(uid, getToken());
//    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
//    responses.insert(m_http, responseSlot);
//    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListRequest()));
//}

void VkApi::audioListResponse() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();

    QByteArray ar = reply -> readAll();
    QJsonObject doc = responseToJson(ar);
    FuncContainer func = responses.take(reply);

    if (doc.contains("error")) {
        qDebug() << ar;
        errorSend(doc, func.obj);
    } else {
        doc = doc.value("response").toObject();
        connect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
        emit audioListReceived(doc);
        disconnect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
    }

    reply -> close();
    delete reply;
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

void VkApi::errorSend(QJsonObject & doc, const QObject * obj) {
    QJsonObject error = doc.value("error").toObject();
    int err_code = error.value("error_code").toInt();
    QString err_msg = error.value("error_msg").toString();

    connect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
    emit errorReceived(err_code, err_msg);
    disconnect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
}

///////////////////////////////////////////////////////////
/// SLOTS
///////////////////////////////////////////////////////////


//void VkApi::audioCountRequest() {}
//void VkApi::audioSearchRequest() {}
//void VkApi::audioCopyRequest() {}
//void VkApi::audioRemoveRequest() {}

//void VkApi::audioSaveServerRequest() {}
//void VkApi::audioSave() {}

//void VkApi::audioAlbumsRequest() {}
//void VkApi::audioAlbumAddRequest() {}
//void VkApi::audioAlbumEditRequest() {}
//void VkApi::audioAlbumRemoveRequest() {}
//void VkApi::audioAlbumMoveToRequest() {}
