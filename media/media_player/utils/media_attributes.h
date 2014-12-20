#ifndef MEDIA_ATTRIBUTES_H
#define MEDIA_ATTRIBUTES_H

#include "media_player_utils.h"
#include <qhash>
#include <qstring>

struct AttributeStream {
    AttributeStream(int i) {index = i;}

    int index;
    QString language;
    QHash<QString, QString> attrs;
};

struct MediaAttributes {
    MediaAttributes(AVFormatContext * context) : hasAudio(false), hasVideo(false), hasSubtitles(false), hasAttachment(false) {
        if (context) {
            AVDictionaryEntry * tag = 0;
            while ((tag = av_dict_get(context -> metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
                common.insert(tag -> key, tag -> value);
            }

            AVDictionary * dict;
            AttributeStream * stream;

            for(uint i = 0; i < context -> nb_streams; i++) {
                if (context -> streams[i] -> codec -> codec_type == AVMEDIA_TYPE_DATA || context -> streams[i] -> codec -> codec_type == AVMEDIA_TYPE_UNKNOWN)
                    continue;

                stream = new AttributeStream(i);

                dict = context -> streams[i] -> metadata;
                while ((tag = av_dict_get(dict, "", tag, AV_DICT_IGNORE_SUFFIX))) {
                    if (QString(tag -> key) == "language")
                        stream -> language = tag -> value;
                    else
                        stream -> attrs.insert(tag -> key, tag -> value);
                }

                switch(context -> streams[i] -> codec -> codec_type) {
                    case AVMEDIA_TYPE_VIDEO:        { hasVideo = true; videos.append(stream); break;}
                    case AVMEDIA_TYPE_AUDIO:        { hasAudio = true; audios.append(stream); break;}
                    case AVMEDIA_TYPE_SUBTITLE:     { hasSubtitles = true; subtitles.append(stream); break;}
                    case AVMEDIA_TYPE_ATTACHMENT:   { hasAttachment = true; delete stream; break;}
                    default: { delete stream; break;}
                }
            }
        }
    }

    ~MediaAttributes() {
        qDeleteAll(audios);
        qDeleteAll(videos);
        qDeleteAll(subtitles);
    }

    int streamIndexOfLang(AVMediaType stream_type, QList<QString> langs) {
        QList<AttributeStream *> & pointer = audios;

        switch(stream_type) {
            case AVMEDIA_TYPE_VIDEO:        { pointer = videos; }
            case AVMEDIA_TYPE_SUBTITLE:     { pointer = subtitles; }
            default: { return -1; }
        }

        foreach(AttributeStream * stream, pointer)
            foreach(QString lang, langs)
                if (stream -> language == lang)
                    return stream -> index;

        return -1;
    }

    QHash<QString, QString> common;
    QList<AttributeStream *> audios;
    QList<AttributeStream *> videos;
    QList<AttributeStream *> subtitles;

    bool hasAudio;
    bool hasVideo;
    bool hasSubtitles;
    bool hasAttachment;
};

#endif // MEDIA_ATTRIBUTES_H
