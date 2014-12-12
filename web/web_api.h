#ifndef WEB_API_H
#define WEB_API_H

#include <QObject>
//#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QPixmap>

#include "media/media_player/utils/web/custom_network_access_manager.h"

class WebApi : public QObject {
    Q_OBJECT   
public:
    WebApi();
    ~WebApi();

    QString getError();

    virtual QString name() const = 0;
    virtual QString authUrl() const = 0;
    virtual QString proceedAuthResponse(const QUrl & url) = 0;

    virtual bool isConnected() { return false; }

    CustomNetworkAccessManager * manager() const;
    CustomNetworkAccessManager * createManager();

    QPixmap openRemoteImage(QString url);
    QNetworkReply * syncRequest(QNetworkReply * m_http);

    void clearData();
    void addFriend(QString uid, QString name);
    void addGroup(QString uid, QString name);

    QHash<QString, QString> friendsList() const;
    QHash<QString, QString> groupsList() const;

    void fromJson(QJsonObject & hash);
    QJsonObject & toJson(QJsonObject & root);

protected:
    QJsonObject responseToJson(QByteArray data);
//    QDomDocument toXml(QByteArray data);

    CustomNetworkAccessManager * netManager;

    QHash<QString, QString> friends;
    QHash<QString, QString> groups;

    QString error;
};

#endif // WEB_API_H
