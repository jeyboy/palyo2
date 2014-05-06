#include "level_tree_view.h"
#include <QDebug>

LevelTreeView::LevelTreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new LevelTreeModel(hash)), parent, settingsSet) {
}

LevelTreeView::~LevelTreeView() {

}

QModelIndex LevelTreeView::dropProcession(const QList<QUrl> & list) {
    ModelItem * index = model -> root();
    filesRoutine(index, list);
    return model -> index(index);
}

void LevelTreeView::filesRoutine(ModelItem * index, QFileInfo currFile){
    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        model -> appendRow(createItem(file.fileName(), index));
    }

    QFileInfoList folderList = folderDirectories(currFile);

    foreach(QFileInfo file, folderList) {
        filesRoutine(model -> addFolder(file.filePath(), model -> root()), file);
    }
}

void LevelTreeView::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            filesRoutine(model -> addFolder(file.filePath(), model -> root()), file);
        } else {
            model -> appendRow(createItem(file.fileName(), index));
        }
    }
}
