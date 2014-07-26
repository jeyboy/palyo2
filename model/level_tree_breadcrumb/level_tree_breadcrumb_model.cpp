#include "level_tree_breadcrumb_model.h"
#include <QDebug>

LevelTreeBreadcrumbModel::LevelTreeBreadcrumbModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {

}

LevelTreeBreadcrumbModel::~LevelTreeBreadcrumbModel() {

}

QString LevelTreeBreadcrumbModel::buildBreadcrumb(QFileInfo file) {
    QString folderName;
    if (file.isDir())
        folderName = file.filePath();
    else
        folderName = file.dir().path();

    QStringList breadcrumbs = folderName.split('/', QString::SkipEmptyParts);
    return breadcrumbs.join(" > ");
}

void LevelTreeBreadcrumbModel::removeFolderPrebuild(ModelItem * temp) {
    temp -> parent() -> removeFolder(temp -> data(TITLEID).toString());
    temp -> parent() -> removeChildren(temp -> row(), 1);
}

QModelIndex LevelTreeBreadcrumbModel::dropProcession(const QList<QUrl> & list) {
    QFileInfo file = QFileInfo(list.first().toLocalFile());
    QString folderName;

    folderName = buildBreadcrumb(file);

    ModelItem * newIndex = addFolder("", folderName, root());

    filesRoutine(newIndex, list);
    return index(newIndex);
}

void LevelTreeBreadcrumbModel::filesRoutine(ModelItem * index, QFileInfo currFile) {
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
    ModelItem * newFolder;

    foreach(QFileInfo file, folderList) {
        newFolder = addFolder("", buildBreadcrumb(file), root());
        filesRoutine(newFolder, file);
        if (newFolder -> childCount() == 0)
            removeFolderPrebuild(newFolder);
    }


    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        appendRow(createItem(file.filePath(), index));
    }
}

void LevelTreeBreadcrumbModel::filesRoutine(ModelItem * index, QList<QUrl> list) {
    ModelItem * newFolder;
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            newFolder = addFolder("", buildBreadcrumb(file), root());
            filesRoutine(newFolder, file);
            if (newFolder -> childCount() == 0)
                removeFolderPrebuild(newFolder);
        } else {
            if (Extensions::instance() -> respondToExtension(file.suffix()))
                appendRow(createItem(file.filePath(), index));
        }
    }
}
