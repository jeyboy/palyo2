#ifndef LEVEL_TREE_BREADCRUMB_VIEW_H
#define LEVEL_TREE_BREADCRUMB_VIEW_H

#include "model/view.h"
#include "level_tree_breadcrumb_model.h"

class LevelTreeBreadcrumbView : public View {
  Q_OBJECT
public:
    LevelTreeBreadcrumbView(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~LevelTreeBreadcrumbView();
};
#endif // LEVEL_TREE_BREADCRUMB_VIEW_H
