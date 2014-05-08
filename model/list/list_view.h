#ifndef LIST_VIEW_H
#define LIST_VIEW_H

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
#include "list_model.h"

//#ifndef CBHASH
//#define CBHASH

//  #include <QHash>
//  typedef QHash <QString, int> CBHash;
//  Q_DECLARE_METATYPE(CBHash);
//#endif // CBHASH

class ListView : public View {
  Q_OBJECT
public:
    ListView(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~ListView();

protected:
    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};

#endif // LIST_VIEW_H
