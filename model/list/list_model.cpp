#include "list_model.h"
#include <QDebug>

ListModel::ListModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {

}

ListModel::~ListModel() {

}

QModelIndex ListModel::dropProcession(const QList<QUrl> & list) {
    ModelItem * newIndex = root();
    filesRoutine(newIndex, list);
    return index(newIndex);
}

void ListModel::filesRoutine(ModelItem * index, QFileInfo currFile){
    QFileInfoList folderList = folderDirectories(currFile);

    foreach(QFileInfo file, folderList) {
        filesRoutine(index, file);
    }

    QFileInfoList fileList = folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        appendRow(createItem(file.filePath(), index));
    }
}

void ListModel::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            filesRoutine(index, file);
        } else {
            appendRow(createItem(file.filePath(), index));
        }
    }
}
