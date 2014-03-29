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
            if (fileInfo.exists()) {
                QIcon res = instance()->iconProvider.icon(fileInfo);
                instance()->icons->insert(extension, res);
                return res;
            } else {
                return missedIcon();
            }
        }
    }
}

QIcon IconProvider::missedIcon() {
    return QIcon(":file_error");
}

QIcon IconProvider::dirIcon() {
    return instance()->iconProvider.icon(QFileIconProvider::Folder);
}
