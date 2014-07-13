#include "level_tree_view.h"
#include <QDebug>

LevelTreeView::LevelTreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new LevelTreeModel(hash)), parent, settingsSet) {
}

LevelTreeView::~LevelTreeView() {

}

QModelIndex LevelTreeView::dropProcession(const QList<QUrl> & list) {
    QFileInfo file = QFileInfo(list.first().toLocalFile());
    QString fName;

    if (file.isDir()) {
        fName = file.fileName();
    } else {
        fName = folderName(file);
    }

    ModelItem * index = model -> addFolder("", fName, model -> root());

    filesRoutine(index, list);
    if (index -> childCount() == 0) {
        model -> removeFolderPrebuild(index);
        return QModelIndex();
    } else return model -> index(index);
}

void LevelTreeView::filesRoutine(ModelItem * index, QFileInfo currFile) {
    QFileInfoList folderList = folderDirectories(currFile);
    ModelItem * newFolder;
    bool already_exist;

    foreach(QFileInfo file, folderList) {
        already_exist = model -> isFolderExist(file.fileName(), model -> root());
        newFolder = model -> addFolder("", file.fileName(), model -> root());
        filesRoutine(newFolder, file);
        if (!already_exist && newFolder -> childCount() == 0)
            model -> removeFolderPrebuild(newFolder);
    }


    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        model -> appendRow(createItem(file.filePath(), index));
    }
}

void LevelTreeView::filesRoutine(ModelItem * index, QList<QUrl> list) {
    ModelItem * newFolder;
    bool already_exist;

    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            already_exist = model -> isFolderExist(file.fileName(), model -> root());
            newFolder = model -> addFolder("", file.fileName(), model -> root());
            filesRoutine(newFolder, file);
            if (!already_exist && newFolder -> childCount() == 0)
                model -> removeFolderPrebuild(newFolder);
        } else {
            model -> appendRow(createItem(file.filePath(), index));
        }
    }
}
