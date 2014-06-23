#ifndef VK_API_PRIVATE_H
#define VK_API_PRIVATE_H

//#include "web/web_api.h"
//#include "misc/func_container.h"
//#include "model/model_item.h"
//#include "web/auth_chemas/teu_auth.h"

#include <QUrl>
#include <QUrlQuery>
#include <QStringList>

class VkApiPrivate {
public:
    static QString authUrl();
    static QUrl wallUrl(QString & uid, QString token, int & offset, int & count);
    static QUrl audioRefreshUrl(QStringList uids, QString token);
    static QUrl audioAlbumsUrl(QString & uid, QString token, int offset);
    static QUrl audioInfoUrl(QString & uid, QString currUid, QString token);

protected:
    static QString apiVersion();
    static QUrlQuery methodParams(QString & token);
    static QString getAPIUrl();


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
};

#endif // VK_API_PRIVATE_H
