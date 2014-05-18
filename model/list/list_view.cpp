#include "list_view.h"
#include <QDebug>

ListView::ListView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new ListModel(hash)), parent, settingsSet) {
}

ListView::~ListView() {

}

QModelIndex ListView::dropProcession(const QList<QUrl> & list) {
    ModelItem * index = model -> root();
    filesRoutine(index, list);
    return model -> index(index);
}

void ListView::filesRoutine(ModelItem * index, QFileInfo currFile){
    QFileInfoList folderList = folderDirectories(currFile);

    foreach(QFileInfo file, folderList) {
        filesRoutine(index, file);
    }

    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        model -> appendRow(createItem(file.filePath(), index));
    }
}

void ListView::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            filesRoutine(index, file);
        } else {
            model -> appendRow(createItem(file.filePath(), index));
        }
    }
}
