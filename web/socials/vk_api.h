#ifndef VK_API_H
#define VK_API_H

#include "web/web_api.h"
#include "misc/func_container.h"
#include "model/model_item.h"

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

    void getWallAttachmentsList(FuncContainer responseSlot, QString uid = "0");

    void getAudioList(FuncContainer responseSlot, QString uid = "0");
    void refreshAudioList(FuncContainer slot, QHash<ModelItem *, QString> uids);

    ~VkApi() { }

    static VkApi * instance();
    static VkApi * instance(QJsonObject obj);
    static void close() {
        delete self;
    }

    void clearData();
    void addFriend(QString uid, QString name);
    void addGroup(QString uid, QString name);

    QHash<QString, QString> friendsList() const;
    QHash<QString, QString> groupsList() const;

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

signals:
    void audioListReceived(QJsonObject &);
    void audioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &);
    void errorReceived(int, QString &);

protected:
    QString apiVersion() const;
    QUrlQuery methodParams();
    QString getAPIUrl();
    void errorSend(QJsonObject & doc, const QObject * obj);

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
    void wallRequest();

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
    VkApi(QJsonObject hash) : WebApi() {
        fromJson(hash);
    }

    VkApi() : WebApi() {
//        token = "";
//        expires_in = "";
//        user_id = "";
    }

    static VkApi *self;

    QString token;
    QString expires_in;
    QString user_id;

    QHash<QString, QString> friends;
    QHash<QString, QString> groups;

    QHash<QNetworkReply *, FuncContainer> responses;
    QHash<QNetworkReply *, QHash<ModelItem *, QString> > collations;
};

#endif // VK_API_H
