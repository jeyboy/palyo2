#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "web/socials/soundcloud_api.h"
#include "model/model.h"
#include "model/tree/tree_model.h"
#include "media/duration.h"

class SoundcloudModel : public TreeModel {
    Q_OBJECT

public:
    SoundcloudModel(QString uid, QJsonObject * hash = 0, QObject *parent = 0);
    ~SoundcloudModel();

//    ModelItem * buildPath(QString path);
    WebApi * getApi() { return SoundcloudApi::instance(); }
    QString getTabUid() const;
public slots:
    void refresh();

protected slots:
    void proceedAudioList(QJsonObject &);
    void proceedAudioList(QJsonArray &, ModelItem *, QHash<ModelItem*, QString> &);
    void errorReceived(int, QString &);
protected:
    QString tabUid;
};


#endif // SOUNDCLOUD_MODEL_H
