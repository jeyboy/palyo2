#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "model/view.h"
#include "model/tree/tree_view.h"
#include "model/web/vk/vk_model.h"

class VkView : public TreeView {
  Q_OBJECT
public:
    VkView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    ~VkView();

//    QJsonObject toJSON();

    QModelIndex dropProcession(const QList<QUrl> & list);

    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
protected:
    void dropEvent(QDropEvent *event);
};

#endif // VK_VIEW_H
