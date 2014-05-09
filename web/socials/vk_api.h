#ifndef VK_API_H
#define VK_API_H

#include "web/web_api.h"

class VkApi : public WebApi {
    Q_OBJECT
public:
    QString name() const;
    QString authUrl() const;
    QString proceedAuthResponse(const QUrl & url);

    void setParams(QString accessToken, QString userID, QString expiresIn);

    QString getToken();
    QString getExpire();
    QString getUserID();

    void getUserAudioList(int uid = -1);
//    void getGroupAudioList(int uid = -1);
//    void getAlbumAudioList(int uid = -1);

    ~VkApi() { }

    static VkApi * instance();
    static void close() {
        delete self;
    }

signals:
    void audioListReceived(QJsonObject &);

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
    void getAudioList(int uid = -1);

    VkApi() : WebApi() {
        token = QString();
        user_id = QString();
        expires_in = QString();
    }

    static VkApi *self;

    QString token;
    QString expires_in;
    QString user_id;
};

#endif // VK_API_H
