#include "web/web_api.h"

WebApi::WebApi() {
    netManager = createManager();
}

WebApi::~WebApi() {
    delete netManager;
}

QString WebApi::getError() {
    return error;
}

CustomNetworkAccessManager * WebApi::manager() const {
    return netManager;
}

CustomNetworkAccessManager * WebApi::createManager() {
    return new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
}

QByteArray WebApi::openRemoteImage(QString url) {
    CustomNetworkAccessManager * currManager = createManager();
    QNetworkReply * reply;
    QVariant possibleRedirectUrl;
    QByteArray ar;

    while(true) {
        reply = currManager -> get(QNetworkRequest(QUrl(url)));
        reply = syncRequest(reply);
        possibleRedirectUrl = reply -> attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (possibleRedirectUrl.isValid()) {
            url = possibleRedirectUrl.toUrl().toString();
            reply -> close();
            delete reply;
        } else {
            ar = reply -> readAll();
            reply -> close();
            currManager;
            break;
        }
    }

    return ar;
}

QNetworkReply * WebApi::syncRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return m_http;
}

void WebApi::clearData() {
    friends.clear();
    groups.clear();
}

void WebApi::addFriend(QString uid, QString name) {
    friends.insert(uid, name);
}
void WebApi::addGroup(QString uid, QString name) {
    groups.insert(uid, name);
}

QHash<QString, QString> WebApi::friendsList() const {
    return friends;
}
QHash<QString, QString> WebApi::groupsList() const {
    return groups;
}

void WebApi::fromJson(QJsonObject & hash) {
    QJsonObject ar = hash.value("friends").toObject();

    foreach(QString key, ar.keys()) {
        addFriend(key, ar.value(key).toString());
    }

    ar = hash.value("groups").toObject();

    foreach(QString key, ar.keys()) {
        addGroup(key, ar.value(key).toString());
    }
}
QJsonObject & WebApi::toJson(QJsonObject & root) {
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

QJsonObject WebApi::responseToJson(QByteArray data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}
//QDomDocument WebApi::toXml(QByteArray data) {
//    QDomDocument dom;
//    dom.setContent(data);
//    return dom;

////    QDomElement  root         = dom.firstChildElement(); // <response> root element
////    QDomNode  audioElement = root.firstChildElement(); // <audio>

////    while(!audioElement.isNull()){
////         QString url =  audioElement
////                            .toElement()
////                            .elementsByTagName("url")
////                            .item(0)
////                            .toElement()  //<url>
////                            .text();
////         list.append(url);
////         audioElement = audioElement.nextSibling(); //next element
////    }
//}
