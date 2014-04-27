#ifndef WEB_API_H
#define WEB_API_H

#include <QObject>
//#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QUrl>
#include "custom_network_access_manager.h"

class WebApi : public QObject {
    Q_OBJECT   
public:
    WebApi();
    ~WebApi();

    CustomNetworkAccessManager * manager() const;

protected:
    QJsonObject toJson(QByteArray data);
//    QDomDocument toXml(QByteArray data);

    QByteArray sendRequest(QString sendMethod, QString request, QHttpMultiPart * parts = 0);
    CustomNetworkAccessManager * netManager;
};

#endif // WEB_API_H
