#ifndef LEVEL_TREE_MODEL_H
#define LEVEL_TREE_MODEL_H

#include "model/model.h"

class LevelTreeModel : public Model {
    Q_OBJECT

public:
    LevelTreeModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~LevelTreeModel();

protected:
    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};

#endif // LEVEL_TREE_MODEL_H
