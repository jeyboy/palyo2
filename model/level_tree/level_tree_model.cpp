#include "level_tree_model.h"
#include <QDebug>

LevelTreeModel::LevelTreeModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {

}

LevelTreeModel::~LevelTreeModel() {

}

QModelIndex LevelTreeModel::dropProcession(const QList<QUrl> & list) {
    QFileInfo file = QFileInfo(list.first().toLocalFile());
    QString fName;

    if (file.isDir()) {
        fName = file.fileName();
    } else {
        fName = folderName(file);
    }

    ModelItem * newIndex = addFolder("", fName, root());

    filesRoutine(newIndex, list);
    if (newIndex -> childCount() == 0) {
        removeFolderPrebuild(newIndex);
        return QModelIndex();
    } else return index(newIndex);
}

void LevelTreeModel::filesRoutine(ModelItem * index, QFileInfo currFile) {
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
    ModelItem * newFolder;
    bool already_exist;

    foreach(QFileInfo file, folderList) {
        already_exist = isFolderExist(file.fileName(), root());
        newFolder = addFolder("", file.fileName(), root());
        filesRoutine(newFolder, file);
        if (!already_exist && newFolder -> childCount() == 0)
            removeFolderPrebuild(newFolder);
    }


    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        appendRow(createItem(file.filePath(), index));
    }
}

void LevelTreeModel::filesRoutine(ModelItem * index, QList<QUrl> list) {
    ModelItem * newFolder;
    bool already_exist;

    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            already_exist = isFolderExist(file.fileName(), root());
            newFolder = addFolder("", file.fileName(), root());
            filesRoutine(newFolder, file);
            if (!already_exist && newFolder -> childCount() == 0)
                removeFolderPrebuild(newFolder);
        } else {
            appendRow(createItem(file.filePath(), index));
        }
    }
}

