#include "mediainfo.h"
#include "qdebug.h"

MediaInfo::MediaInfo(QString filepath) {
    fileName = filepath.toStdWString();
    TagLib::FileRef f(fileName.c_str(), false, TagLib::AudioProperties::Fast);

    artist = QString::fromStdWString(f.tag() -> artist().toWString());
    title = QString::fromStdWString(f.tag() -> title().toWString());
    album = QString::fromStdWString(f.tag() -> album().toWString());
    genre = QString::fromStdWString(f.tag() -> genre().toWString());
    year = f.tag() -> year();
    track = f.tag() -> track();
}

void MediaInfo::initInfo() {
    TagLib::FileRef f(fileName.c_str(), true, TagLib::AudioProperties::Accurate);
    bitrate = f.audioProperties() -> bitrate();
    channels = f.audioProperties() -> channels();
    length = f.audioProperties() -> length();
    sampleRate = f.audioProperties() -> sampleRate();
}

QString MediaInfo::getArtist() const { return artist; }
QString MediaInfo::getTitle() const { return title; }
QString MediaInfo::getAlbum() const { return album; }
QString MediaInfo::getGenre() const { return genre; }

int MediaInfo::getYear() const { return year; }
int MediaInfo::getTrack() const { return track; }
int MediaInfo::getChannels() const { return channels; }
int MediaInfo::getBitrate() const { return bitrate; }
int MediaInfo::getLength() const { return length; }
int MediaInfo::getSampleRate() const { return sampleRate; }
