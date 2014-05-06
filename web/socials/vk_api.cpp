#include "web/socials/vk_api.h"

VkApi *VkApi::self = 0;

VkApi *VkApi::instance() {
    if(!self)
        self = new VkApi();
    return self;
}

QString VkApi::name() const { return "vk"; }
void VkApi::setParams(QString accessToken, QString userID, QString expiresIn) {
    token = accessToken;
    user_id = userID;
    expires_in = expiresIn;
}

QString VkApi::getToken() {
    return token;
}
QString VkApi::getExpire() {
    return expires_in;
}
QString VkApi::getUserID() {
    return user_id;
}

QString VkApi::authUrl() const {
    QUrl url("https://oauth.vk.com/authorize");
    QUrlQuery queryParams = QUrlQuery();
    queryParams.addQueryItem("v", apiVersion());
    queryParams.addQueryItem("display", "page");
    queryParams.addQueryItem("client_id", "4332211");
    queryParams.addQueryItem("response_type", "token");
    queryParams.addQueryItem("scope", "audio,video,friends,offline");
    queryParams.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");

    url.setQuery(queryParams);
    return url.toString();

//    return "https://oauth.vk.com/authorize?client_id=4332211&scope=audio,video,friends,offline&redirect_uri=https://oauth.vk.com/blank.html&display=page&v=5.21&response_type=token";
}

QString VkApi::proceedResponse(const QUrl & url) {
    QUrlQuery query(url.fragment());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("access_token")) {\
        token = query.queryItemValue("access_token");
        expires_in = query.queryItemValue("expires_in");
        user_id = query.queryItemValue("user_id");
        return "accept";
    }

    return "";
}


///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

QString VkApi::apiVersion() const {
    return "5.21";
}

QUrlQuery VkApi::methodParams() {
    QUrlQuery query = QUrlQuery();

    query.addQueryItem("v", apiVersion());
    query.addQueryItem("access_token", getToken());

    return query;
}

QString VkApi::getAPIUrl() {
    return "https://api.vk.com/method/";
}

//QUrl VkApi::getAudioListUrl() const { return QUrl(getAPIUrl() + "audio.get"); }
//QUrl VkApi::getAudioCountUrl() const { return QUrl(getAPIUrl() + "audio.getCount"); }
//QUrl VkApi::getAudioSearchUrl() const { return QUrl(getAPIUrl() + "audio.search"); }
//QUrl VkApi::getAudioCopyUrl() const { return QUrl(getAPIUrl() + "audio.add"); }
//QUrl VkApi::getAudioRemoveUrl() const { return QUrl(getAPIUrl() + "audio.delete"); }

//QUrl VkApi::getAudioAlbumsListUrl() const { return QUrl(getAPIUrl() + "audio.getAlbums"); }
//QUrl VkApi::getAudioAlbumAddUrl() const { return QUrl(getAPIUrl() + "audio.addAlbum"); }
//QUrl VkApi::getAudioAlbumEditUrl() const { return QUrl(getAPIUrl() + "audio.editAlbum"); }
//QUrl VkApi::getAudioAlbumRemoveUrl() const { return QUrl(getAPIUrl() + "audio.deleteAlbum"); }
//QUrl VkApi::getAudioAlbumMoveToUrl() const { return QUrl(getAPIUrl() + "audio.moveToAlbum"); }

//QUrl VkApi::getAudioSaveServerUrl() const { return QUrl(getAPIUrl() + "audio.getUploadServer"); }
//QUrl VkApi::getAudioSaveUrl() const { return QUrl(getAPIUrl() + "audio.save"); }

///////////////////////////////////////////////////////////
/// SLOTS
///////////////////////////////////////////////////////////

void VkApi::audioListRequest() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();

    QJsonObject doc = toJson(reply -> readAll());
    qDebug() << doc;
    qDebug() << reply -> readAll();
}
//void VkApi::audioCountRequest() {}
//void VkApi::audioSearchRequest() {}
//void VkApi::audioCopyRequest() {}
//void VkApi::audioRemoveRequest() {}

//void VkApi::audioSaveServerRequest() {}
//void VkApi::audioSave() {}

//void VkApi::audioAlbumsRequest() {}
//void VkApi::audioAlbumAddRequest() {}
//void VkApi::audioAlbumEditRequest() {}
//void VkApi::audioAlbumRemoveRequest() {}
//void VkApi::audioAlbumMoveToRequest() {}

///////////////////////////////////////////////////////////

void VkApi::getAudioList(int uid) {
    QUrl url(getAPIUrl() + "execute");
    QUrlQuery query = methodParams();
    query.addQueryItem("code",
//                       "var audioList = [];" +
//                       "var count = API.audio.getCount()@.response; " +
//                       "var step = 0;" +
//                       "while(step < count) { audioList.pust(API.audio.get())};"
                       );
    url.setQuery(query);

    QNetworkReply * m_http = manager -> get(QNetworkRequest(url));
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListRequest()));
}