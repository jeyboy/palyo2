#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "model/tree/tree_view.h"
#include "model/web/vk/vk_model.h"

//#ifndef CBHASH
//#define CBHASH

//  #include <QHash>
//  typedef QHash <QString, bool> CBHash;
//  Q_DECLARE_METATYPE(CBHash);
//#endif // CBHASH

class VkView : public TreeView {
  Q_OBJECT
public:
    VkView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    ~VkView();
};

#endif // VK_VIEW_H
