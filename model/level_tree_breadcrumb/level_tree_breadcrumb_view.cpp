#include "level_tree_breadcrumb_view.h"
#include <QDebug>

LevelTreeBreadcrumbView::LevelTreeBreadcrumbView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new LevelTreeBreadcrumbModel(hash)), parent, settingsSet) {
}

LevelTreeBreadcrumbView::~LevelTreeBreadcrumbView() {

}

QString LevelTreeBreadcrumbView::buildBreadcrumb(QFileInfo file) {
    QString folderName;
    if (file.isDir())
        folderName = file.filePath();
    else
        folderName = file.dir().path();

    QStringList breadcrumbs = folderName.split('/', QString::SkipEmptyParts);
    return breadcrumbs.join(" > ");
}

void LevelTreeBreadcrumbView::removeFolderPrebuild(ModelItem * temp) {
    temp -> parent() -> removeFolder(temp -> data(TITLEID).toString());
    temp -> parent() -> removeChildren(temp -> row(), 1);
}

QModelIndex LevelTreeBreadcrumbView::dropProcession(const QList<QUrl> & list) {
    QFileInfo file = QFileInfo(list.first().toLocalFile());
    QString folderName;

    folderName = buildBreadcrumb(file);

    ModelItem * index = model -> addFolder("", folderName, model -> root());

    filesRoutine(index, list);
    return model -> index(index);
}

void LevelTreeBreadcrumbView::filesRoutine(ModelItem * index, QFileInfo currFile) {
    QFileInfoList folderList = folderDirectories(currFile);
    ModelItem * newFolder;

    foreach(QFileInfo file, folderList) {
        newFolder = model -> addFolder("", buildBreadcrumb(file), model -> root());
        filesRoutine(newFolder, file);
        if (newFolder -> childCount() == 0)
            removeFolderPrebuild(newFolder);
    }


    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        model -> appendRow(createItem(file.filePath(), index));
    }
}

void LevelTreeBreadcrumbView::filesRoutine(ModelItem * index, QList<QUrl> list) {
    ModelItem * newFolder;
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            newFolder = model -> addFolder("", buildBreadcrumb(file), model -> root());
            filesRoutine(newFolder, file);
            if (newFolder -> childCount() == 0)
                removeFolderPrebuild(newFolder);
        } else {
            model -> appendRow(createItem(file.filePath(), index));
        }
    }
}
