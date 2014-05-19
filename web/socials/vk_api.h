#ifndef VK_API_H
#define VK_API_H

#include "web/web_api.h"
#include "misc/ip_check.h"

class VkApi : public WebApi {
    Q_OBJECT
public:
    QString name() const;
    QString authUrl() const;
    QString proceedAuthResponse(const QUrl & url);

    void setParams(QString accessToken, QString userID, QString expiresIn, QString requestIp);

    QString getToken();
    QString getExpire();
    QString getUserID();
    QString getIp();
    QHash<int, QString> getGenres() const;

    void getUserAudioList(QString uid = "0");
//    void getGroupAudioList(QString uid = "0");
//    void getAlbumAudioList(QString uid = "0");

    ~VkApi() { }

    static VkApi * instance();
    static VkApi * instance(QString pToken, QString pUserId, QString pExpired, QString currIp);
    static void close() {
        delete self;
    }

signals:
    void audioListReceived(QJsonObject &);
    void errorReceived(QJsonObject &);

protected:
    QString apiVersion() const;
    QUrlQuery methodParams();
    QString getAPIUrl();

//    QUrl getAudioListUrl() const;
//    QUrl getAudioCountUrl() const;
//    QUrl getAudioSearchUrl() const;
//    QUrl getAudioCopyUrl() const;
//    QUrl getAudioRemoveUrl() const;

//    QUrl getAudioAlbumsListUrl() const;
//    QUrl getAudioAlbumAddUrl() const;
//    QUrl getAudioAlbumEditUrl() const;
//    QUrl getAudioAlbumRemoveUrl() const;
//    QUrl getAudioAlbumMoveToUrl() const;

//    QUrl getAudioSaveServerUrl() const;
//    QUrl getAudioSaveUrl() const;

protected slots:
    void audioListRequest();
//    void audioCountRequest();
//    void audioSearchRequest();
//    void audioCopyRequest();
//    void audioRemoveRequest();

//    void audioSaveServerRequest();
//    void audioSave();

//    void audioAlbumsRequest();
//    void audioAlbumAddRequest();
//    void audioAlbumEditRequest();
//    void audioAlbumRemoveRequest();
//    void audioAlbumMoveToRequest();

private:   
    void getAudioList(QString uid = "0");
    void init(QString pToken = "", QString pUserId = "", QString pExpired = "");

    VkApi(QString pToken, QString pUserId, QString pExpired, QString currIp) : WebApi(currIp) {
        init(pToken, pUserId, pExpired);
    }

    VkApi() : WebApi() {
        init();
    }

    static VkApi *self;

    QString token;
    QString expires_in;
    QString user_id;

    QHash<int, QString> genres;
};

#endif // VK_API_H
