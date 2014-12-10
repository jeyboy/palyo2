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

    WebObject(void * related_to, QUrl & url);
    ~WebObject();

    QString & lastError() const;

    void open();
    bool openSync();
    qint64 lenght() const;
    int read(void * buf, int & length);
    int seek(qint64 newPos = -1);

protected:
    void downloadProc(QUrl savePath);
    void initRequest();
    void closeConnection();

signals:
    void start(void *);
    void end(void *, bool);
    void progress(void *, float percentage);

protected slots:
    void download(QUrl savePath);
    void proceedResponse();
    void onError(QNetworkReply::NetworkError);
    void onError(QString);

private:
    void * relation;
    int bufferLength;
    QString error;
    QNetworkReply * m_http;
    QUrl obj_url;
    static CustomNetworkAccessManager webManager;
};

//WebObject WebObject::webManager = CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);

#endif // WEB_OBJECT_H
