#include "soundcloud_view.h"
#include <QDebug>

SoundcloudView::SoundcloudView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : TreeView(dynamic_cast<Model *>(new SoundcloudModel(QString::number(settingsSet.value("uid")), hash)), parent, settingsSet) {
}

SoundcloudView::~SoundcloudView() {}

QJsonObject SoundcloudView::toJSON() {
    QJsonObject res = TreeView::toJSON();

    res.insert("uid", ((SoundcloudModel *)model) -> getTabUid());

    return res;
}

QModelIndex SoundcloudView::dropProcession(const QList<QUrl> & list) {
//    ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path());
//    filesRoutine(index, list);
//    return model -> index(index);
}

void SoundcloudView::filesRoutine(ModelItem * index, QFileInfo currFile){
//    QFileInfoList fileList = folderFiles(currFile);

//    foreach(QFileInfo file, fileList) {
//        FileItem * fi = new FileItem(file.fileName(), index);
//        model -> appendRow(fi -> toModelItem());
//    }

//    QFileInfoList folderList = folderDirectories(currFile);

//    foreach(QFileInfo file, folderList) {
//        ModelItem * new_index = model -> addFolder(file.fileName(), index);
//        filesRoutine(new_index, file);
//    }
}

void SoundcloudView::filesRoutine(ModelItem * index, QList<QUrl> list){
//    foreach(QUrl url, list) {
//        QFileInfo file = QFileInfo(url.toLocalFile());
//        if (file.isDir()) {
//            ModelItem * new_index = model -> addFolder(file.fileName(), index);
//            filesRoutine(new_index, file);
//        } else {
//            FileItem * fi = new FileItem(file.fileName(), index);
//            model -> appendRow(fi -> toModelItem());
//        }
//    }
}

void SoundcloudView::dropEvent(QDropEvent *event) {
    event -> ignore();

//    if (event -> source() != this && event -> mimeData() -> hasUrls()) {
//        QModelIndex modelIndex = dropProcession(event -> mimeData() -> urls());
//        model -> refresh();
//        scrollTo(modelIndex);
//        expand(modelIndex);
//        event -> accept();
//    } else event -> ignore();
}
