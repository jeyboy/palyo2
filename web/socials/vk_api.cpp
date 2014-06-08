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

QString VkApi::name() const { return "vk"; }
void VkApi::setParams(QString accessToken, QString userID, QString expiresIn) {
    token = accessToken;
    user_id = userID;
    expires_in = expiresIn;
}

QString VkApi::getToken() {
    return token;
}
QString VkApi::getExpire() {
    return expires_in;
}
QString VkApi::getUserID() {
    return user_id;
}

void VkApi::clearData() {
    friends.clear();
    groups.clear();
}

void VkApi::addFriend(QString uid, QString name) {
    friends.insert(uid, name);
}
void VkApi::addGroup(QString uid, QString name) {
    groups.insert(uid, name);
}

QHash<QString, QString> VkApi::friendsList() const {
    return friends;
}
QHash<QString, QString> VkApi::groupsList() const {
    return groups;
}

void VkApi::fromJson(QJsonObject hash) {
    user_id = hash.value("_u_").toString();
    token = hash.value("_t_").toString();
    expires_in = hash.value("_e_").toString();

    QJsonObject ar = hash.value("friends").toObject();

    foreach(QString key, ar.keys()) {
        addFriend(key, ar.value(key).toString());
    }

    ar = hash.value("groups").toObject();

    foreach(QString key, ar.keys()) {
        addGroup(key, ar.value(key).toString());
    }
}
QJsonObject VkApi::toJson() {
    QJsonObject root;

    root["_u_"] = getUserID();
    root["_t_"] = getToken();
    root["_e_"] = getExpire();

    QJsonObject friendsJson;
    foreach(QString key, friends.keys()) {
        friendsJson.insert(key, QJsonValue(friends.value(key)));
    }
    root.insert("friends", friendsJson);

    QJsonObject groupsJson;
    foreach(QString key, groups.keys()) {
        groupsJson.insert(key, QJsonValue(groups.value(key)));
    }
    root.insert("groups", groupsJson);

    return root;
}

bool VkApi::isConnected() const {
    return !token.isEmpty() && !user_id.isEmpty();
}

///////////////////////////////////////////////////////////
/// LINKS
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
QString VkApi::authUrl() const {
    QUrl url("https://oauth.vk.com/authorize");
    QUrlQuery queryParams = QUrlQuery();
    queryParams.addQueryItem("v", apiVersion());
    queryParams.addQueryItem("display", "page");
    queryParams.addQueryItem("client_id", "4332211");
    queryParams.addQueryItem("response_type", "token");
    queryParams.addQueryItem("scope", "audio,video,friends,groups,offline");
    queryParams.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");

    url.setQuery(queryParams);
    return url.toString();
}

QString VkApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.fragment());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("access_token")) {\
        token = query.queryItemValue("access_token");
        expires_in = query.queryItemValue("expires_in");
        user_id = query.queryItemValue("user_id");
        return "accept";
    }

    return "";
}

///////////////////////////////////////////////////////////
/// WALL
///////////////////////////////////////////////////////////

void VkApi::getWallAttachmentsList(FuncContainer responseSlot, QString uid) {
    uid = uid == "0" ? getUserID() : uid;
    QUrl url(getAPIUrl() + "execute");
    QUrlQuery query = methodParams();

    query.addQueryItem("code",
                           QString("var limit = 100;") +
                           "var iterator = limit; var response = [];" +
                           "var posts = API.wall.get({count: limit, owner_id: " + uid + "});" +
                           "var count = posts.count, post_items = posts.items;" +

                           "while (iterator < count) { post_items.push(API.wall.get({count: limit, offset: iterator, owner_id: " + uid + "}).items); iterator = iterator %2b limit;}" +

                           "while(post_items.length > 0) {" +
                           "  var curr = post_items.pop();" +
                           "  var audios = curr.attachments@.audio %2b curr.copy_history[0].attachments@.audio %2b curr.copy_history[1].attachments@.audio;" +
                           "  if (audios.length > 0) {" +
                           "    response.unshift({title: curr.text, date: curr.date, audios: audios});" +
                           " }" +
                           "}" +

                           "return {date: posts.items[0].date, posts: response};"
                       );

    url.setQuery(query);

    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    responses.insert(m_http, responseSlot);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(wallRequest()));
}

void VkApi::wallRequest() {
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
    QUrl url(getAPIUrl() + "execute");
    QUrlQuery query = methodParams();
    QStringList uidList(uids.values());

    query.addQueryItem("code",
                       "return API.audio.getById({audios: \"" + uidList.join(',') + "\"});"
                       );
    url.setQuery(query);

    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    responses.insert(m_http, responseSlot);
    collations.insert(m_http, uids);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListRequest()));
}

void VkApi::getAudioList(FuncContainer responseSlot, QString uid) {
    uid = uid == "0" ? getUserID() : uid;
    QUrl url(getAPIUrl() + "execute");
    QUrlQuery query = methodParams();

    if (uid == getUserID()) {
        query.addQueryItem("code",
                           QString("var curr;") +
                           "var groups = API.groups.get({owner_id: " + uid + ", count: 1000, extended: 1}).items;" +
                           "var proceed_groups = [];" +
                           "while(groups.length > 0) { curr = groups.pop();  proceed_groups.push({id: curr.id, title: curr.name}); };" +

                           "var friends = API.friends.get({user_id: " + uid + ", order: \"name\", fields: \"nickname\"});" +
                           "var proceed_friends = [];" +
                           "if (friends.count > 0) {while(friends.items.length > 0) { curr = friends.items.pop();  proceed_friends.push({id: curr.id, title: curr.first_name %2b \" \" %2b curr.last_name }); }; };" +

                           "var folders_result = API.audio.getAlbums({count: 20, owner_id: " + uid + "});" +
                           "var folders_count = folders_result.count;" +
                           "var proceed_folders = {};" +
                           "if (folders_count > 0) {while(folders_result.items.length > 0) { curr = folders_result.items.pop();  proceed_folders.push(" +
                           "{folder_id: curr.id, title: curr.title, items: API.audio.get({album_id: curr.id}).items});" +
                           "}; };" +
                           "return {audio_list: API.audio.get({count: 6000, owner_id: " + uid + "}), albums: proceed_folders, groups: proceed_groups, friends: proceed_friends, albums_count: folders_count};"
                           );
    } else {
        query.addQueryItem("code",
                           "var folders_result = API.audio.getAlbums({count: 20, owner_id: " + uid + "});" +
                           "var folders_count = folders_result.count;" +
                           "var sort_by_folders = {};" +
                           "if (folders_count > 0) {while(folders_result.items.length > 0) { var curr = folders_result.items.pop();  sort_by_folders.push(" +
                           "{folder_id: curr.id, title: curr.title, items: API.audio.get({owner_id: " + uid + ", album_id: curr.id}).items});" +
                           "}; };" +
                           "return {audio_list: API.audio.get({count: 6000, owner_id: " + uid + "}), albums: sort_by_folders, albums_count: folders_count};"
                           );
    }
    url.setQuery(query);

    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    responses.insert(m_http, responseSlot);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListRequest()));
}

//void VkApi::getAudioAlbums(FuncContainer responseSlot, QString uid, int offset) {
//    QUrl url(getAPIUrl() + "execute");
//    QUrlQuery query = methodParams();
//    int req_count = 20;

//    query.addQueryItem("code",
//                       QString("var curr;") +
//                       "var folders_result = API.audio.getAlbums({count: " + QString::number(req_count) + ", offset: " + QString::number(offset) + ", owner_id: " + uid + "});" +
//                       "var folders_count = folders_result.count;" +
//                       "var folders_result = folders_result.items;" +
//                       "var proceed_folders = {};" +
//                       "while(folders.length > 0) { curr = folders.pop();  proceed_folders.push(" +
//                       "{folder_id: curr.id, title: curr.title, items: API.audio.get({owner_id: " + uid + ", album_id: curr.id}).items});" +
//                       "};" +
//                       "return {albums: proceed_folders, albums_count: folders_count, albums_offset: " + QString::number(req_count + offset) + "};"
//                       );
//    url.setQuery(query);

//    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
//    responses.insert(m_http, responseSlot);
//    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListRequest()));
//}

void VkApi::audioListRequest() {
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







///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

QString VkApi::apiVersion() const {
    return "5.21";
}

QUrlQuery VkApi::methodParams() {
    QUrlQuery query = QUrlQuery();

    query.addQueryItem("v", apiVersion());
    query.addQueryItem("access_token", getToken());

    return query;
}

QString VkApi::getAPIUrl() {
    return "https://api.vk.com/method/";
}

void VkApi::errorSend(QJsonObject & doc, const QObject * obj) {
    QJsonObject error = doc.value("error").toObject();
    int err_code = error.value("error_code").toInt();
    QString err_msg = error.value("error_msg").toString();

    connect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
    emit errorReceived(err_code, err_msg);
    disconnect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
}

//QUrl VkApi::getAudioListUrl() const { return QUrl(getAPIUrl() + "audio.get"); }
//QUrl VkApi::getAudioCountUrl() const { return QUrl(getAPIUrl() + "audio.getCount"); }
//QUrl VkApi::getAudioSearchUrl() const { return QUrl(getAPIUrl() + "audio.search"); }
//QUrl VkApi::getAudioCopyUrl() const { return QUrl(getAPIUrl() + "audio.add"); }
//QUrl VkApi::getAudioRemoveUrl() const { return QUrl(getAPIUrl() + "audio.delete"); }

//QUrl VkApi::getAudioAlbumsListUrl() const { return QUrl(getAPIUrl() + "audio.getAlbums"); }
//QUrl VkApi::getAudioAlbumAddUrl() const { return QUrl(getAPIUrl() + "audio.addAlbum"); }
//QUrl VkApi::getAudioAlbumEditUrl() const { return QUrl(getAPIUrl() + "audio.editAlbum"); }
//QUrl VkApi::getAudioAlbumRemoveUrl() const { return QUrl(getAPIUrl() + "audio.deleteAlbum"); }
//QUrl VkApi::getAudioAlbumMoveToUrl() const { return QUrl(getAPIUrl() + "audio.moveToAlbum"); }

//QUrl VkApi::getAudioSaveServerUrl() const { return QUrl(getAPIUrl() + "audio.getUploadServer"); }
//QUrl VkApi::getAudioSaveUrl() const { return QUrl(getAPIUrl() + "audio.save"); }

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
