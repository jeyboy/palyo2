#ifndef LEVEL_TREE_VIEW_H
#define LEVEL_TREE_VIEW_H

#include "model/view.h"
#include "level_tree_model.h"

//#ifndef CBHASH
//#define CBHASH

//  #include <QHash>
//  typedef QHash <QString, int> CBHash;
//  Q_DECLARE_METATYPE(CBHash);
//#endif // CBHASH

class LevelTreeView : public View {
  Q_OBJECT
public:
    LevelTreeView(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~LevelTreeView();

protected:
    void removeFolderPrebuild(ModelItem * temp);
    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};
#endif // LEVEL_TREE_VIEW_H
