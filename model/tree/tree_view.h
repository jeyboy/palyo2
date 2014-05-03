#ifndef TREE_VIEW_H
#define TREE_VIEW_H

//#include <QMimeData>
//#include <QTreeView>
//#include <QMouseEvent>
//#include <QPixmap>
//#include <QFileInfo>
//#include <QJsonObject>
//#include <QDrag>
//#include <QHeaderView>
//#include <QDesktopServices>
//#include <QMenu>
//#include <QMessageBox>

#include "model/view.h"
#include "model/tree/tree_model.h"
#include "model/model_item.h"

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, bool> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH

class TreeView : public View {
  Q_OBJECT
public:
    TreeView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    ~TreeView();

protected:
    Model * newModel(QJsonObject * hash = 0);

    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfoList list);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};

#endif // TREE_VIEW_H
