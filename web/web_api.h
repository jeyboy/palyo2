#ifndef WEB_API_H
#define WEB_API_H

#include <QObject>
//#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QUrl>
#include "override/custom_network_access_manager.h"

class WebApi : public QObject {
    Q_OBJECT   
public:
    WebApi();
    ~WebApi();

    QString getError();

    virtual QString name() const = 0;
    virtual QString authUrl() const = 0;
    virtual QString proceedAuthResponse(const QUrl & url) = 0;

    void downloadFile(QUrl uri, QUrl savePath);

    CustomNetworkAccessManager * manager() const;

protected slots:
    void downloadConnectionResponsed();

signals:
    void downloadProgress(QUrl & uri, int percentDone);
    void downloadFinished(QUrl & uri);
    void downloadError(QUrl & uri, QString message);

protected:
    QJsonObject toJson(QByteArray data);
//    QDomDocument toXml(QByteArray data);

//    QByteArray sendRequest(QString sendMethod, QString request, QHttpMultiPart * parts = 0);
    CustomNetworkAccessManager * netManager;

    QString error;

    QHash<void *, QUrl> * downloads;
};

#endif // WEB_API_H
