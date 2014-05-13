#ifndef WEB_API_H
#define WEB_API_H

#include <QObject>
//#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QUrl>

#include "override/custom_network_access_manager.h"

struct DownloadPosition {
    DownloadPosition(void * obj, QUrl path) {
        item = obj;
        savePath = path;
    }

    void * item;
    QUrl savePath;
};

class WebApi : public QObject {
    Q_OBJECT   
public:
    WebApi();
    ~WebApi();

    QString getError();

    virtual QString name() const = 0;
    virtual QString authUrl() const = 0;
    virtual QString proceedAuthResponse(const QUrl & url) = 0;

    void downloadFile(QObject * caller, void * item, QUrl uri, QUrl savePath);

    CustomNetworkAccessManager * manager() const;

protected slots:
    void downloadConnectionResponsed();

signals:
    void downloadProgress(void * item, int percentDone);
    void downloadFinished(void * item, bool success);
    void downloadError(void * item, QString message);

protected:
    QJsonObject toJson(QByteArray data);
//    QDomDocument toXml(QByteArray data);

//    QByteArray sendRequest(QString sendMethod, QString request, QHttpMultiPart * parts = 0);
    CustomNetworkAccessManager * netManager;

    QString error;

    QHash<void *, DownloadPosition *> * downloads;
};

#endif // WEB_API_H
