#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "web/socials/soundcloud_api.h"
#include "model/web/web_model.h"
#include "media/duration.h"

class SoundcloudModel : public WebModel {
    Q_OBJECT

public:
    SoundcloudModel(QString uid, QJsonObject * hash = 0, QObject *parent = 0);
    ~SoundcloudModel();

    WebApi * getApi() { return SoundcloudApi::instance(); }
public slots:
    void refresh();

protected slots:
    void proceedResponse(QJsonObject &);
    void proceedResponse(QJsonArray &, ModelItem *, QHash<ModelItem*, QString> &);
};

#endif // SOUNDCLOUD_MODEL_H
