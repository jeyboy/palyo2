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

QJsonObject WebApi::toJson(QByteArray data) {
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

QByteArray WebApi::sendRequest(QString sendMethod, QString request, QHttpMultiPart * parts) {
    QNetworkReply * reply;
    QNetworkRequest req(request);

    if (sendMethod == "get") {
        reply = netManager -> get(req);
    } else if (sendMethod == "post") {
        reply = netManager -> post(req, parts);
    } else if (sendMethod == "put") {
        reply = netManager -> put(req, parts);
    } if (sendMethod == "delete") {
        reply = netManager -> deleteResource(req);
    }

    return reply -> readAll();
}
