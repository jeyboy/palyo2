#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "taglib.h"
#include "tag.h"
#include "fileref.h"

#include <QString>

class MediaInfo {
public:
    MediaInfo(QString filepath, bool onlyTags = true);
    void initInfo();

    QString getArtist() const;
    QString getTitle() const;
    QString getAlbum() const;
    QString getGenre() const;

    int getYear() const;
    int getTrack() const;
    int getChannels() const;
    int getBitrate() const;
    int getLength() const;
    int getSampleRate() const;
private:
    void readInfo(TagLib::FileRef f);

    std::wstring fileName;

    QString artist;
    QString title;
    QString album;
    QString genre;

    int year;
    int track;

    int channels;
    int bitrate;
    int length;
    int sampleRate;
};

#endif // MEDIAINFO_H
