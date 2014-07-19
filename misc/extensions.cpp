#include "extensions.h"

Extensions *Extensions::self = 0;

Extensions *Extensions::instance() {
    if(!self)
        self = new Extensions();
    return self;
}

QFileInfoList Extensions::folderFiles(QFileInfo file) {
    return QDir(file.filePath()).entryInfoList(filters.value(activeFilter), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
}

QFileInfoList Extensions::folderDirectories(QFileInfo file) {
    return QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
//    return QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
}
