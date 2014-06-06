#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "web/ip_checker.h"
#include "web/socials/vk_api.h"
#include "model/model.h"
#include "model/tree/tree_model.h"
#include "media/duration.h"

class VkModel : public TreeModel {
    Q_OBJECT

public:
    VkModel(QString uid, QJsonObject * hash = 0, QObject *parent = 0);
    ~VkModel();

//    ModelItem * buildPath(QString path);
    WebApi * getApi() { return VkApi::instance(); }
    QString getTabUid() const;
public slots:
    void refresh();

protected slots:
    void proceedAudioList(QJsonObject &);
    void proceedAudioList(QJsonArray &, ModelItem *, QHash<ModelItem*, QString> &);
    void proceedAudioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &);
    void errorReceived(QJsonObject &);
protected:
    QString tabUid;
};


#endif // VK_MODEL_H
