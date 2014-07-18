#ifndef LEVEL_TREE_BREADCRUMB_MODEL_H
#define LEVEL_TREE_BREADCRUMB_MODEL_H

#include "model/model.h"

class LevelTreeBreadcrumbModel : public Model {
    Q_OBJECT

public:
    LevelTreeBreadcrumbModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~LevelTreeBreadcrumbModel();
protected:
    QString buildBreadcrumb(QFileInfo);
    void removeFolderPrebuild(ModelItem * temp);
    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};

#endif // LEVEL_TREE_BREADCRUMB_MODEL_H
