#include "mediainfo.h"
#include "qdebug.h"

MediaInfo *MediaInfo::self = 0;

MediaInfo *MediaInfo::instance() {
    if(!self)
        self = new MediaInfo();
    return self;
}

QString MediaInfo::getInfo(QString filepath) {
//    TagLib::FileRef f("C:\\Josh Whelchel - Demoninution.mp3");
    TagLib::FileRef f(filepath.toStdWString().c_str());
//    qDebug() << "-- TAG (basic) --" << endl;
    qDebug() << "title   - \"" << QString::fromStdWString(f.tag()->title().toWString())   << "\"" << endl;
    qDebug() << "artist  - \"" << QString::fromStdWString(f.tag()->artist().toWString())  << "\"" << endl;
    qDebug() << "album   - \"" << QString::fromStdWString(f.tag()->album().toWString())   << "\"" << endl;
    qDebug() << "year    - \"" << f.tag()->year()    << "\"" << endl;
    qDebug() << "comment - \"" << QString::fromStdWString(f.tag()->comment().toWString()) << "\"" << endl;
    qDebug() << "track   - \"" << f.tag()->track()   << "\"" << endl;
    qDebug() << "genre   - \"" << QString::fromStdWString(f.tag()->genre().toWString())   << "\"" << endl;
}
