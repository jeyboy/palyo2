#include "tree_view.h"
#include <QDebug>

TreeView::TreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new TreeModel(hash)), parent, settingsSet) {
}

TreeView::~TreeView() {

}

QModelIndex TreeView::dropProcession(const QList<QUrl> & list) {
    ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path()); // -> parent();
    filesRoutine(index, list);
    return model -> index(index);
}

void TreeView::filesRoutine(ModelItem * index, QFileInfoList list){
    foreach(QFileInfo file, list) {
        if (file.isDir()) {
            ModelItem * new_index = model -> addFolder(file.fileName(), index);
            filesRoutine(new_index, folderEntities(file));
        } else {
            model -> appendRow((new FileItem(file.filePath(), index)) -> toModelItem());
        }
    }
}

void TreeView::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            ModelItem * new_index = model -> addFolder(file.fileName(), index);
            filesRoutine(new_index, folderEntities(file));
        } else {
            model -> appendRow((new FileItem(file.filePath(), index)) -> toModelItem());
        }
    }
}
