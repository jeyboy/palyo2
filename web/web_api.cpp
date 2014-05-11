#include "web/web_api.h"

WebApi::WebApi() {
    netManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
    downloads = new QHash<void *, QUrl>();
}

WebApi::~WebApi() {
    delete netManager;
}

QString WebApi::getError() {
    return error;
}

void WebApi::downloadFile(QUrl uri, QUrl savePath) {
    QNetworkReply * m_http = manager() -> get(QNetworkRequest(uri));
    downloads -> insert(m_http, savePath);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(downloadConnectionResponsed()));
}

void WebApi::downloadConnectionResponsed() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();
    QUrl savePath = downloads -> value(reply);

    qDebug() << reply -> bytesAvailable();
    qDebug() << reply -> isSequential();

    QFile file(savePath.toLocalFile());

//    QIODevice::Append | QIODevice::Unbuffered

    if (!file.open(QIODevice::WriteOnly)) {
        emit downloadError(savePath, "File did not opened");
        return;
    }

    if (!file.resize(reply -> bytesAvailable())) {
        emit downloadError(savePath, "Empty space on your device is not enough");
        return;
    }

    QByteArray buffer;
    qint64 pos = 0;
    double limit = reply -> bytesAvailable();
    int bufferLength = 1024 * 1024 * 1; //1 mb

    while(!reply -> atEnd()) {
        try {
            buffer = reply -> read(bufferLength);
            pos += buffer.length();
            file.write(buffer);

            qDebug() << (((pos/limit) * 100)) << "%";
            emit downloadProgress(savePath, (pos/limit) * 100);
        }

        catch(...) {
            qDebug() << "Some shit";
        }
    }

    file.close();
    reply -> close();
    emit downloadFinished(savePath);
    delete reply;
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

//QByteArray WebApi::sendRequest(QString sendMethod, QString request, QHttpMultiPart * parts) {
//    QNetworkReply * reply;
//    QNetworkRequest req(request);

//    if (sendMethod == "get") {
//        reply = netManager -> get(req);
//    } else if (sendMethod == "post") {
//        reply = netManager -> post(req, parts);
//    } else if (sendMethod == "put") {
//        reply = netManager -> put(req, parts);
//    } if (sendMethod == "delete") {
//        reply = netManager -> deleteResource(req);
//    }

//    return reply -> readAll();
//}
