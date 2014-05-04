#include "tree_view.h"
#include <QDebug>

TreeView::TreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new TreeModel(hash)), parent, settingsSet) {
}

TreeView::~TreeView() {

}

QModelIndex TreeView::dropProcession(const QList<QUrl> & list) {
    ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path());
    filesRoutine(index, list);
    return model -> index(index);
}

void TreeView::filesRoutine(ModelItem * index, QFileInfo currFile){
    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        FileItem * fi = new FileItem(file.fileName(), index);
        model -> appendRow(fi -> toModelItem());
    }

    QFileInfoList folderList = folderDirectories(currFile);

    foreach(QFileInfo file, folderList) {
        ModelItem * new_index = model -> addFolder(file.fileName(), index);
        filesRoutine(new_index, file);
    }
}

void TreeView::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            ModelItem * new_index = model -> addFolder(file.fileName(), index);
            filesRoutine(new_index, file);
        } else {
            FileItem * fi = new FileItem(file.fileName(), index);
            model -> appendRow(fi -> toModelItem());
        }
    }
}
