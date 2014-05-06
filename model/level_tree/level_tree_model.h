#ifndef LEVEL_TREE_MODEL_H
#define LEVEL_TREE_MODEL_H

#include "model/model.h"

class LevelTreeModel : public Model {
    Q_OBJECT

public:
    LevelTreeModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~LevelTreeModel();
};

#endif // LEVEL_TREE_MODEL_H
