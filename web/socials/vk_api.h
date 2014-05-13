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

    bool isRefreshRequire() const;

    void getUserAudioList(QString uid = "0");
//    void getGroupAudioList(QString uid = "0");
//    void getAlbumAudioList(QString uid = "0");

    ~VkApi() { }

    static VkApi * instance();
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

    VkApi() : WebApi() {
        token = QString();
        user_id = QString();
        expires_in = QString();

        genres = QHash<int, QString>();

        genres.insert(1, "Rock");
        genres.insert(2, "Pop");
        genres.insert(3, "Rap & Hip-Hop");
        genres.insert(4, "Easy Listening");
        genres.insert(5, "Dance & House");
        genres.insert(6, "Instrumental");
        genres.insert(7, "Metal");
        genres.insert(8, "Dubstep");
        genres.insert(9, "Jazz & Blues");
        genres.insert(10, "Drum & Bass");
        genres.insert(11, "Trance");
        genres.insert(12, "Chanson");
        genres.insert(13, "Ethnic");
        genres.insert(14, "Acoustic & Vocal");
        genres.insert(15, "Reggae");
        genres.insert(16, "Classical");
        genres.insert(17, "Indie Pop");
        genres.insert(18, "Other");
        genres.insert(19, "Speech");

        genres.insert(21, "Alternative");
        genres.insert(22, "Electropop & Disco");

//        ip = IpCheck::getIp();
//        qDebug() << "IP: " << ip;
        refreshRequire = false;
    }

    static VkApi *self;

    QString token;
    QString expires_in;
    QString user_id;

    QHash<int, QString> genres;

    bool refreshRequire;
};

#endif // VK_API_H
