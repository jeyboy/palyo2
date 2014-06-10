#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "web/web_api.h"
#include "misc/func_container.h"
#include "model/model_item.h"

class SoundcloudApi : public WebApi {
    Q_OBJECT
public:
    QString name() const;
    QString authUrl() const;
    QUrl authTokenUrl() const;
    QByteArray authTokenUrlParams(QString code) const;
    QString proceedAuthResponse(const QUrl & url);

    void setParams(QString accessToken, QString userID, QString expiresIn);

    QString getClientId() const;
    QString getToken();
    QString getExpire();
    QString getUserID();

    void getAudioList(FuncContainer responseSlot, QString uid = "0");

    ~SoundcloudApi() { }

    static SoundcloudApi * instance();
    static SoundcloudApi * instance(QJsonObject obj);
    static void close() {
        delete self;
    }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    bool isConnected() const;

signals:
    void audioListReceived(QJsonObject &);
    void errorReceived(int, QString &);

protected:
    QUrlQuery methodParams();
    QString getAPIUrl();
    void errorSend(QJsonObject & doc, const QObject * obj);

protected slots:
    void audioListRequest();
private:   
    SoundcloudApi(QJsonObject hash) : WebApi() {
        fromJson(hash);
    }

    SoundcloudApi() : WebApi() {

    }

    static SoundcloudApi *self;

    QString token;
    QString expires_in;
    QString user_id;

    QHash<QNetworkReply *, FuncContainer> responses;
};

#endif // SOUNDCLOUD_API_H
