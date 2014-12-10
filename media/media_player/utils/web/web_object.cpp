#include "web_object.h"

WebObject::WebObject(QUrl & url) : bufferLength(1024 * 1024) /*1 mb*/ {
    obj_url = url;
}

WebObject::~WebObject() {
    if (m_http) {
        m_http -> close();
        delete m_http;
        m_http = 0;
    }
}

QString & WebObject::lastError() const { return error; }

void WebObject::open() {
    initRequest();
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(proceedResponse()));
}

bool WebObject::openSync() {
    initRequest();

    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool state = m_http -> isOpen();
    emit end(state);
    return state;
}

void WebObject::download(QUrl savePath) {
//    downloader = new QFutureWatcher<QNetworkReply *>();
//    connect(downloader, SIGNAL(finished()), this, SLOT(finishDownload()));
//    downloader -> setFuture(QtConcurrent::run(this, &Download::downloading, reply));
    QtConcurrent::run(this, &WebObject::downloadProc, savePath);
}

void WebObject::downloadProc(QUrl savePath) {
    emit start();

    if (!m_http && !openSync())
        return;

    if (!savePath.isLocalFile()) {
        emit onError("File: remote saving is not supported");
        return;
    }

    QFile file(savePath.toLocalFile());

//    QIODevice::Append | QIODevice::Unbuffered

    if (!file.open(QIODevice::WriteOnly)) {
        emit onError("File: cannot open file for writing");
    } else {
        if (!file.resize(m_http -> bytesAvailable())) {
            emit onError("File: free memory on disk is not enough");
        } else {
            QByteArray buffer;
            qint64 pos = 0;
            double limit = m_http -> bytesAvailable();

            while(!m_http -> atEnd()) {
                try {
                    buffer = m_http -> read(bufferLength);
                    pos += buffer.length();
                    file.write(buffer);

                    emit progress((pos / limit) * 100);
                }

                catch(...) {
                    emit onError(QNetworkReply::UnknownNetworkError);
                }
            }

            file.close();
            emit end(true);
        }
    }
}

void WebObject::initRequest() {
    emit start();
    closeConnection();
    m_http = manager() -> get(QNetworkRequest(obj_url));
}

void WebObject::closeConnection() {
    if (m_http) {
        m_http -> close();
        delete m_http;
        m_http = 0;
    }
}

void WebObject::proceedResponse() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();
    QVariant possibleRedirectUrl = reply -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        delete reply;

        QUrl url = possibleRedirectUrl.toUrl();
        open(url);
    } else {
        emit end(true);
    }
}

void WebObject::onError(QString er) {
    error = er;
    emit end(false);
}

void WebObject::onError(QNetworkReply::NetworkError er) {
    switch (er) {
        case QNetworkReply::NoError: { error = ""; }

        case QNetworkReply::ConnectionRefusedError: { error = "Connection: refused"; }
        case QNetworkReply::RemoteHostClosedError: { error = "Connection: host closed"; }
        case QNetworkReply::HostNotFoundError: { error = "Connection: host not found"; }
        case QNetworkReply::TimeoutError: { error = "Connection: timeout"; }
        case QNetworkReply::OperationCanceledError: { error = "Connection: operation canceled"; }
        case QNetworkReply::TemporaryNetworkFailureError: { error = "Connection: network is not accessible"; }
        case QNetworkReply::NetworkSessionFailedError: { error = "Connection: network session is not accessible"; }
        case QNetworkReply::BackgroundRequestNotAllowedError: { error = "Connection: background request not allowed"; }
        case QNetworkReply::UnknownNetworkError: { error = "Unknow error"; }

        case QNetworkReply::ProxyConnectionRefusedError: { error = "Proxy: connection refused"; }
        case QNetworkReply::ProxyConnectionClosedError: { error = "Proxy: connection closed"; }
        case QNetworkReply::ProxyNotFoundError: { error = "Proxy: not found"; }
        case QNetworkReply::ProxyTimeoutError: { error = "Proxy: timeout"; }
        case QNetworkReply::ProxyAuthenticationRequiredError: { error = "Proxy: authentication required"; }
        case QNetworkReply::UnknownProxyError: { error = "Proxy: unknow error"; }

        case QNetworkReply::ContentAccessDenied: { error = "Content: access denied"; }
        case QNetworkReply::ContentOperationNotPermittedError: { error = "Content: operation not permitted"; }
        case QNetworkReply::ContentNotFoundError: { error = "Content: not found"; }
        case QNetworkReply::AuthenticationRequiredError: { error = "Content: authentication required"; }
        case QNetworkReply::ContentReSendError: { error = "Content: resend required"; } //TODO: maybe auto resend ?
        case QNetworkReply::ContentConflictError: { error = "Content: state conflict"; } //TODO: maybe auto resend ?
        case QNetworkReply::ContentGoneError: { error = "Content: is gone"; }
        case QNetworkReply::UnknownContentError: { error = "Content: unknow error"; }

        case QNetworkReply::ProtocolUnknownError: { error = "Protocol: unknow"; }
        case QNetworkReply::ProtocolInvalidOperationError: { error = "Protocol: invalid operation"; }
        case QNetworkReply::ProtocolFailure: { error = "Protocol: failure"; }

        case QNetworkReply::InternalServerError: { error = "Server: internal error"; }
        case QNetworkReply::OperationNotImplementedError: { error = "Server: operation not implemented"; }
        case QNetworkReply::ServiceUnavailableError: { error = "Server: service unavailable"; }
        case QNetworkReply::UnknownServerError: { error = "Server: unknow error"; }
    }

    emit end(false);
}
