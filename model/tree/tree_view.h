#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "model/view.h"
#include "model/tree/tree_model.h"

class TreeView : public View {
  Q_OBJECT
public:
    TreeView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    TreeView(Model * newModel, QWidget *parent, CBHash settingsSet);
    ~TreeView();

protected:
    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};

#endif // TREE_VIEW_H
