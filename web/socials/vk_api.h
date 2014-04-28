#ifndef VK_API_H
#define VK_API_H

#include "web/web_api.h"

class VkApi : public WebApi {
public:
    QString name() const;
    QString authUrl() const;
    QString proceedResponse(const QUrl & url);

    void setParams(QString accessToken, QString userID, QString expiresIn);

    QString getToken();
    QString getExpire();
    QString getUserID();

    ~VkApi() {

    }

    static VkApi * instance();
    static void close() {
        delete self;
    }

private:
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
