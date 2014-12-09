#include "web_object.h"

WebObject::WebObject(QUrl & url) {
    open(url);
}

WebObject::~WebObject() {

}

QString & WebObject::lastError() const { return error; }

void WebObject::open(QUrl & url) {
    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(proceedResponse()));
}

void WebObject::proceedResponse() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();
    QVariant possibleRedirectUrl = reply -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        delete reply;

        QUrl url = possibleRedirectUrl.toUrl();
        open(url);
    } else {
        emit response(this);
    }
}

void WebObject::onError(QNetworkReply::NetworkError er) {
    switch (er) {
        case QNetworkReply:NoError: { error = ""; }

        case QNetworkReply:ConnectionRefusedError: { error = "Connection: refused"; }
        case QNetworkReply:RemoteHostClosedError: { error = "Connection: host closed"; }
        case QNetworkReply:HostNotFoundError: { error = "Connection: host not found"; }
        case QNetworkReply:TimeoutError: { error = "Connection: timeout"; }
        case QNetworkReply:OperationCanceledError: { error = "Connection: operation canceled"; }
        case QNetworkReply:TemporaryNetworkFailureError: { error = "Connection: network is not accessible"; }
        case QNetworkReply:NetworkSessionFailedError: { error = "Connection: network session is not accessible"; }
        case QNetworkReply:BackgroundRequestNotAllowedError: { error = "Connection: background request not allowed"; }
        case QNetworkReply:UnknownNetworkError: { error = "Unknow error"; }

        case QNetworkReply:ProxyConnectionRefusedError: { error = "Proxy: connection refused"; }
        case QNetworkReply:ProxyConnectionClosedError: { error = "Proxy: connection closed"; }
        case QNetworkReply:ProxyNotFoundError: { error = "Proxy: not found"; }
        case QNetworkReply:ProxyTimeoutError: { error = "Proxy: timeout"; }
        case QNetworkReply:ProxyAuthenticationRequiredError: { error = "Proxy: authentication required"; }
        case QNetworkReply:UnknownProxyError: { error = "Proxy: unknow error"; }

        case QNetworkReply:ContentAccessDenied: { error = "Content: access denied"; }
        case QNetworkReply:ContentOperationNotPermittedError: { error = "Content: operation not permitted"; }
        case QNetworkReply:ContentNotFoundError: { error = "Content: not found"; }
        case QNetworkReply:AuthenticationRequiredError: { error = "Content: authentication required"; }
        case QNetworkReply:ContentReSendError: { error = "Content: resend required"; } //TODO: maybe auto resend ?
        case QNetworkReply:ContentConflictError: { error = "Content: state conflict"; } //TODO: maybe auto resend ?
        case QNetworkReply:ContentGoneError: { error = "Content: is gone"; }
        case QNetworkReply:UnknownContentError: { error = "Content: unknow error"; }

        case QNetworkReply:ProtocolUnknownError: { error = "Protocol: unknow"; }
        case QNetworkReply:ProtocolInvalidOperationError: { error = "Protocol: invalid operation"; }
        case QNetworkReply:ProtocolFailure: { error = "Protocol: failure"; }

        case QNetworkReply:InternalServerError: { error = "Server: internal error"; }
        case QNetworkReply:OperationNotImplementedError: { error = "Server: operation not implemented"; }
        case QNetworkReply:ServiceUnavailableError: { error = "Server: service unavailable"; }
        case QNetworkReply:UnknownServerError: { error = "Server: unknow error"; }
    }

    emit response(0);
}
