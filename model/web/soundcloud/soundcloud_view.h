#ifndef SOUNDCLOUD_VIEW_H
#define SOUNDCLOUD_VIEW_H

#include "model/view.h"
#include "model/tree/tree_view.h"
#include "model/web/soundcloud/soundcloud_model.h"

class SoundcloudView : public TreeView {
  Q_OBJECT
public:
    SoundcloudView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    ~SoundcloudView();

    QJsonObject toJSON();

    QModelIndex dropProcession(const QList<QUrl> & list);

    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
protected:
    void dropEvent(QDropEvent *event);
};

#endif // SOUNDCLOUD_VIEW_H
