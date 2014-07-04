#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "web/ip_checker.h"
#include "web/socials/vk_api.h"
#include "model/web/web_model.h"
#include "media/duration.h"

class VkModel : public WebModel {
    Q_OBJECT

public:
    VkModel(QString uid, QJsonObject * hash = 0, QObject *parent = 0);
    ~VkModel();

    inline WebApi * getApi() { return VkApi::instance(); }
public slots:
    void refresh();
    void refreshWall();

protected slots:
    void proceedWallList(QJsonObject &);

    void proceedAudioList(QJsonObject &);
    void proceedAudioList(QJsonArray &, ModelItem *, QHash<ModelItem*, QString> &);
    void proceedAudioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &);
};


#endif // VK_MODEL_H
