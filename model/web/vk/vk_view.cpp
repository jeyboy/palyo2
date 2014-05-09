#include "vk_view.h"
#include <QDebug>

VkView::VkView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : TreeView(dynamic_cast<Model *>(new VkModel(hash)), parent, settingsSet) {

    QJsonObject res = hash -> value("vk").toObject();

    VkApi::instance() -> setParams(res.value("t").toString(), res.value("u").toString(), res.value("e").toString());
}

VkView::~VkView() {}

QJsonObject VkView::toJSON() {
    QJsonObject res = TreeView::toJSON();

    QJsonObject set = QJsonObject();
    set["u"] = VkApi::instance() -> getUserID();
    set["t"] = VkApi::instance() -> getToken();
    set["e"] = VkApi::instance() -> getExpire();

    res["vk"] = set;
    return res;
}

QModelIndex VkView::dropProcession(const QList<QUrl> & list) {
//    ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path());
//    filesRoutine(index, list);
//    return model -> index(index);
}

void VkView::filesRoutine(ModelItem * index, QFileInfo currFile){
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

void VkView::filesRoutine(ModelItem * index, QList<QUrl> list){
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

void VkView::dropEvent(QDropEvent *event) {
    event -> ignore();

//    if (event -> source() != this && event -> mimeData() -> hasUrls()) {
//        QModelIndex modelIndex = dropProcession(event -> mimeData() -> urls());
//        model -> refresh();
//        scrollTo(modelIndex);
//        expand(modelIndex);
//        event -> accept();
//    } else event -> ignore();
}
