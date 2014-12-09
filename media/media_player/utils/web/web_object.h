#ifndef WEB_OBJECT_H
#define WEB_OBJECT_H

#include <qurl>
#include "custom_network_access_manager.h"

class WebObject : QObject {
    Q_OBJECT
public:
    static CustomNetworkAccessManager & manager() const {
        return webManager;
    }

    WebObject(QUrl & url);
    ~WebObject();

    QString & lastError() const;

protected:
    void open(QUrl & url);

signals:
    void response(void *);

protected slots:
    void proceedResponse();
    void onError(QNetworkReply::NetworkError);

private:
    QString error;
    QNetworkReply * m_http;
    static CustomNetworkAccessManager webManager;
};

//WebObject WebObject::webManager = CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);

#endif // WEB_OBJECT_H
