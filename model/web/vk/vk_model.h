#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "web/socials/vk_api.h"
#include "model/model.h"
#include "model/tree/tree_model.h"

class VkModel : public TreeModel {
    Q_OBJECT

public:
    VkModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~VkModel();

//    ModelItem * buildPath(QString path);
    WebApi * getApi() { return VkApi::instance(); }
protected slots:
    void proceedAudioList(QJsonObject &);
};


#endif // VK_MODEL_H
