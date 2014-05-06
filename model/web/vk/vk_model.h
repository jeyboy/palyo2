#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "web/socials/vk_api.h"
#include "model/model.h"

class VkModel : public Model {
    Q_OBJECT

public:
    VkModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~TreeModel();

    VkModel * buildPath(QString path);
};


#endif // VK_MODEL_H
