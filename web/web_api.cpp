#include "web/web_api.h"

WebApi::WebApi() {
    netManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
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
