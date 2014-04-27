#include "web/socials/vk_api.h"

VkApi *VkApi::self = 0;

VkApi *VkApi::instance() {
    if(!self)
        self = new VkApi();
    return self;
}

QString VkApi::name() const { return "vk"; }
void VkApi::setParams(QString accessToken, QString userID) {
    token = accessToken;
    user_id = userID;
}
