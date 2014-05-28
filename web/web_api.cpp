#include "web/web_api.h"

WebApi::WebApi() {
    netManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
    downloads = new QHash<void *, DownloadPosition *>();
}

WebApi::~WebApi() {
    delete netManager;
}

QString WebApi::getError() {
    return error;
}

void WebApi::downloadFile(QObject * caller, void * item, QUrl uri, QUrl savePath) {
    QNetworkReply * m_http = manager() -> get(QNetworkRequest(uri));
    QCoreApplication::processEvents(QEventLoop::AllEvents, 200);
    downloads -> insert(m_http, new DownloadPosition(item, savePath));
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(downloadConnectionResponsed()));
    QObject::connect(this, SIGNAL(downloadFinished(void *, bool)), caller, SLOT(itemDownloadFinished(void *, bool)));
    QObject::connect(this, SIGNAL(downloadProgress(void *, int)), caller, SLOT(itemDownloadProgress(void *, int)));
}

void WebApi::downloadConnectionResponsed() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();
    DownloadPosition * position = downloads -> value(reply);

    QFile file(position -> savePath.toLocalFile());

//    QIODevice::Append | QIODevice::Unbuffered

    if (!file.open(QIODevice::WriteOnly)) {
        emit downloadError(position -> item, file.errorString());
        emit downloadFinished(position -> item, false);
        return;
    }

    if (!file.resize(reply -> bytesAvailable())) {
        emit downloadError(position -> item, file.errorString());
        emit downloadFinished(position -> item, false);
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

            emit downloadProgress(position -> item, (pos/limit) * 100);
            QCoreApplication::processEvents(QEventLoop::AllEvents, 200);
        }

        catch(...) {
            emit downloadError(position -> item, "Some error occured while download...");
            emit downloadFinished(position -> item, false);
        }
    }

    file.close();
    reply -> close();
    emit downloadFinished(position -> item, true);
    delete reply;
}

CustomNetworkAccessManager * WebApi::manager() const {
    return netManager;
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
