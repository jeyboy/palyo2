#ifndef WEB_OBJECT_H
#define WEB_OBJECT_H

#include <qurl>
#include <qfile>
#include <QtConcurrent/QtConcurrent>
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

    void open();
    bool openSync();

protected:
    void downloadProc(QUrl savePath);
    void initRequest();
    void closeConnection();

signals:
    void start();
    void end(bool);
    void progress(float percentage);

protected slots:
    void download(QUrl savePath);
    void proceedResponse();
    void onError(QNetworkReply::NetworkError);
    void onError(QString);

private:
    int bufferLength;
    QString error;
    QNetworkReply * m_http;
    QUrl obj_url;
    static CustomNetworkAccessManager webManager;
};

//WebObject WebObject::webManager = CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);

#endif // WEB_OBJECT_H
