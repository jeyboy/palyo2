#include "soundcloud_api.h"

SoundcloudApi *SoundcloudApi::self = 0;

SoundcloudApi *SoundcloudApi::instance() {
    if(!self)
        self = new SoundcloudApi();
    return self;
}

SoundcloudApi * SoundcloudApi::instance(QJsonObject obj) {
    if(!self)
        self = new SoundcloudApi(obj);
    else
        SoundcloudApi::instance() -> fromJson(obj);
    return self;
}

QString SoundcloudApi::name() const { return "soundcloud"; }
void SoundcloudApi::setParams(QString accessToken, QString userID, QString expiresIn) {
    token = accessToken;
    user_id = userID;
    expires_in = expiresIn;
}

QString SoundcloudApi::getClientId() const {
    return "8f84790a84f5a5acd1c92e850b5a91b7";
}

QString SoundcloudApi::getToken() {
    return token;
}
QString SoundcloudApi::getExpire() {
    return expires_in;
}
QString SoundcloudApi::getUserID() {
    return user_id;
}

void SoundcloudApi::fromJson(QJsonObject hash) {
    user_id = hash.value("_u_").toString();
    token = hash.value("_t_").toString();
    expires_in = hash.value("_e_").toString();

//    QJsonObject ar = hash.value("friends").toObject();

//    foreach(QString key, ar.keys()) {
//        addFriend(key, ar.value(key).toString());
//    }

//    ar = hash.value("groups").toObject();

//    foreach(QString key, ar.keys()) {
//        addGroup(key, ar.value(key).toString());
//    }
}
QJsonObject SoundcloudApi::toJson() {
    QJsonObject root;

    root["_u_"] = getUserID();
    root["_t_"] = getToken();
    root["_e_"] = getExpire();

//    QJsonObject friendsJson;
//    foreach(QString key, friends.keys()) {
//        friendsJson.insert(key, QJsonValue(friends.value(key)));
//    }
//    root.insert("friends", friendsJson);

//    QJsonObject groupsJson;
//    foreach(QString key, groups.keys()) {
//        groupsJson.insert(key, QJsonValue(groups.value(key)));
//    }
//    root.insert("groups", groupsJson);

    return root;
}

bool SoundcloudApi::isConnected() const {
    return !token.isEmpty();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
QString SoundcloudApi::authUrl() const {
    QUrl url("https://soundcloud.com/connect");
    QUrlQuery queryParams = QUrlQuery();
    queryParams.addQueryItem("client_id", "8f84790a84f5a5acd1c92e850b5a91b7");
//    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
    queryParams.addQueryItem("response_type", "code");
    queryParams.addQueryItem("scope", "non-expiring");
    queryParams.addQueryItem("redirect_uri", "http://sos.com");
    queryParams.addQueryItem("display", "popup");

    url.setQuery(queryParams);
    return url.toString();
}

QUrl SoundcloudApi::authTokenUrl() const {
    QUrl url("https://api.soundcloud.com/oauth2/token");
    return url;
}

QByteArray SoundcloudApi::authTokenUrlParams(QString code) const {
    QUrlQuery queryParams = QUrlQuery();

    queryParams.addQueryItem("client_id", "8f84790a84f5a5acd1c92e850b5a91b7");
    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
    queryParams.addQueryItem("grant_type", "authorization_code");
    queryParams.addQueryItem("redirect_uri", "http://sos.com");
    queryParams.addQueryItem("code", code);

    return queryParams.toString(QUrl::FullyEncoded).toUtf8();
}


//QString SoundcloudApi::authTokenUrl() const {
//    QUrl url("https://api.soundcloud.com/oauth2/token");
//    QUrlQuery queryParams = QUrlQuery();


//    queryParams.addQueryItem("client_id", "8f84790a84f5a5acd1c92e850b5a91b7");
//    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
//    queryParams.addQueryItem("scope", "non-expiring");

////    queryParams.addQueryItem("grant_type", "password");
////    queryParams.addQueryItem("username", "USERNAME");
////    queryParams.addQueryItem("password", "PASSWORD");

//    url.setQuery(queryParams);
//    return url.toString();
//}

QString SoundcloudApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("code")) {
        QNetworkRequest request(authTokenUrl());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkReply * m_http = manager() -> post(request, authTokenUrlParams(query.queryItemValue("code")));
        QEventLoop loop;
        connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        QByteArray ar = m_http -> readAll();
        QJsonObject doc = responseToJson(ar);

        if (doc.contains("access_token")) {
            token = doc.value("access_token").toString();
            qDebug() << "TOKEN " << token;

            QNetworkRequest request("https://api.soundcloud.com/me.json?oauth_token=" + token);
            m_http = manager() -> get(request);
            connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            ar = m_http -> readAll();
            doc = responseToJson(ar);

            qDebug() << doc;

            user_id = QString::number(doc.value("id").toInt());

            return "accept";
        }

        return "reject";
    }

    return "";
}

///////////////////////////////////////////////////////////
/// AUDIO LIST
///////////////////////////////////////////////////////////

void SoundcloudApi::getAudioList(FuncContainer responseSlot, QString uid) {
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

void SoundcloudApi::audioListRequest() {
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

QUrlQuery SoundcloudApi::methodParams() {
    QUrlQuery query = QUrlQuery();

//    query.addQueryItem("v", apiVersion());
//    query.addQueryItem("access_token", getToken());

    return query;
}

QString SoundcloudApi::getAPIUrl() {
    return "https://api.vk.com/method/";
}

void SoundcloudApi::errorSend(QJsonObject & doc, const QObject * obj) {
    QJsonObject error = doc.value("error").toObject();
    int err_code = error.value("error_code").toInt();
    QString err_msg = error.value("error_msg").toString();

    connect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
    emit errorReceived(err_code, err_msg);
    disconnect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
}
