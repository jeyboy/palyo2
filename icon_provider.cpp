#include "icon_provider.h"

IconProvider *IconProvider::self = 0;

IconProvider *IconProvider::instance() {
    if(!self)
        self = new IconProvider();
    return self;
}

//QStyle::SP_DirIcon //folder icon
//QStyle::SP_FileIcon // file icon

QIcon IconProvider::fileIcon(const QString filename, QString extension) {
    if (extension.length() == 0 && filename.length() == 0) {
        return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
    } else {
        if (instance()->icons->contains(extension)) {
            return instance()->icons->value(extension);
        } else {
            QFileInfo fileInfo(filename);
            QIcon res = instance()->iconProvider.icon(fileInfo);
            instance()->icons->insert(extension, res);
            return res;
        }
    }
}

QIcon IconProvider::dirIcon() {
    return instance()->iconProvider.icon(QFileIconProvider::Folder);
}
