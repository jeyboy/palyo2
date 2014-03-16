#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "taglib.h"
#include "tag.h"
#include "fileref.h"
#include <QString>

class MediaInfo {
public:
    static MediaInfo * instance();
    static QString getInfo(QString filepath);

private:
    MediaInfo() {

    }

    static MediaInfo * self;
};

#endif // MEDIAINFO_H
