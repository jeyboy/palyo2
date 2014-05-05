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
    virtual QString proceedResponse(const QUrl & url) = 0;

    CustomNetworkAccessManager * manager() const;

protected:
    QJsonObject toJson(QByteArray data);
//    QDomDocument toXml(QByteArray data);

//    QByteArray sendRequest(QString sendMethod, QString request, QHttpMultiPart * parts = 0);
    CustomNetworkAccessManager * netManager;

    QString error;
};

#endif // WEB_API_H
