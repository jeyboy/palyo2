#ifndef LEVEL_TREE_BREADCRUMB_MODEL_H
#define LEVEL_TREE_BREADCRUMB_MODEL_H

#include "model/model.h"

class LevelTreeBreadcrumbModel : public Model {
    Q_OBJECT

public:
    LevelTreeBreadcrumbModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~LevelTreeBreadcrumbModel();
};

#endif // LEVEL_TREE_BREADCRUMB_MODEL_H
