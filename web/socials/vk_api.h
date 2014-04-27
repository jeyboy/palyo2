#ifndef VK_API_H
#define VK_API_H

#include "web/web_api.h"

class VkApi : public WebApi {
public:
    QString name() const;
    void setParams(QString accessToken, QString userID);

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
    }

    static VkApi *self;

    QString token;
    QString user_id;
};

#endif // VK_API_H
