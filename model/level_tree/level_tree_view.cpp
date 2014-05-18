#include "level_tree_view.h"
#include <QDebug>

LevelTreeView::LevelTreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new LevelTreeModel(hash)), parent, settingsSet) {
}

LevelTreeView::~LevelTreeView() {

}

QModelIndex LevelTreeView::dropProcession(const QList<QUrl> & list) {
    QFileInfo file = QFileInfo(list.first().toLocalFile());
    QString folderName;

    if (file.isDir())
        folderName = file.fileName();
    else
        folderName = file.dir().dirName();

    ModelItem * index = model -> addFolder("", folderName, model -> root());

    filesRoutine(index, list);
    return model -> index(index);
}

void LevelTreeView::filesRoutine(ModelItem * index, QFileInfo currFile){
    QFileInfoList folderList = folderDirectories(currFile);

    foreach(QFileInfo file, folderList) {
        filesRoutine(model -> addFolder("", file.fileName(), model -> root()), file);
    }


    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        model -> appendRow(createItem(file.filePath(), index));
    }
}

void LevelTreeView::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            filesRoutine(model -> addFolder("", file.fileName(), model -> root()), file);
        } else {
            model -> appendRow(createItem(file.filePath(), index));
        }
    }
}
