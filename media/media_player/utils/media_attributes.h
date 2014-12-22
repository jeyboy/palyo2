#ifndef MEDIA_ATTRIBUTES_H
#define MEDIA_ATTRIBUTES_H

#include "media_player_utils.h"
#include "duration.h"
#include <qhash>
#include <qstring>

struct AttributeChapter {
    AttributeChapter(int i, double start, double end) : index(i), start_time(start), end_time(end) {

    }

    int index;
    double start_time, end_time;
    QHash<QString, QString> attrs;
};

struct AttributeStream {
    AttributeStream(int i, QString title) : index(i), name(title) {

    }

    int index;
    QString name;
    QString language;
    QHash<QString, QString> attrs;
};

struct AttributeProgram {
    AttributeProgram(int i, QString title) : index(i), name(title) {

    }

    ~AttributeProgram() {
        qDeleteAll(streams);
    }

    int index;
    QString name;
    QHash<QString, QString> attrs;
    QList<AttributeStream *> streams;
};

struct MediaAttributes {
    MediaAttributes(AVFormatContext * context) : hasAudio(false), hasVideo(false), hasSubtitles(false), hasAttachment(false), is_output(1) {
        if (context) {
            QString lang;


            common.insert("duration",
                context -> duration == AV_NOPTS_VALUE ?
                   "N/A" :
                   Duration::fromSeconds((context -> duration + 5000) / AV_TIME_BASE)
            );

            common.insert("start",
               context -> start_time == AV_NOPTS_VALUE ?
                   "N/A" :
                   Duration::fromSeconds(context -> start_time / AV_TIME_BASE)
            );

            // kb/s
            common.insert("bitrate",
               context -> bit_rate ?
                   QString::number(context -> bit_rate / 1000) :
                   "N/A"
            );



            AttributeChapter * chapter;
            AVChapter * ch;
            for (unsigned int i = 0; i < context -> nb_chapters; i++) {
                ch = context -> chapters[i];
                chapter = new AttributeChapter(i, ch -> start * av_q2d(ch -> time_base), ch -> end * av_q2d(ch -> time_base));
                dumpMetadata(
                   ch -> metadata,
                   chapter -> attrs,
                   lang
                );

                chapters.append(chapter);
            }

            AttributeProgram * program;
            if (context -> nb_programs) {
                int total = 0;
                for (unsigned int j = 0; j < context -> nb_programs; j++) {
                    AVDictionaryEntry * name = av_dict_get(context -> programs[j] -> metadata, "name", NULL, 0);
                    program = new AttributeProgram(context -> programs[j] -> id, QString(name ? name -> value : ""));
                    dumpMetadata(
                       context -> programs[j] -> metadata,
                       program -> attrs,
                       lang
                    );


                    for (unsigned int k = 0; k < context -> programs[j] -> nb_stream_indexes; k++)
                        program -> streams.append(dumpStream(context, context -> programs[j] -> stream_index[k], is_output));

                    total += context -> programs[j] -> nb_stream_indexes;
                }
//                if (total < ic->nb_streams)
//                av_log(NULL, AV_LOG_INFO, " No Program\n");
            }


            dumpMetadata(
               context -> metadata,
               common,
               lang
            );


            AttributeStream * stream;
            for(uint i = 0; i < context -> nb_streams; i++) {
                if (context -> streams[i] -> codec -> codec_type == AVMEDIA_TYPE_DATA || context -> streams[i] -> codec -> codec_type == AVMEDIA_TYPE_UNKNOWN)
                    continue;

                stream = dumpStream(context, i, is_output);

//                stream =  new AttributeStream(i, QString(context -> streams[i] -> codec -> codec_name));

////                av_get_bits_per_sample(context -> streams[i] -> codec -> codec_id);

//                dumpMetadata(
//                            context -> streams[i] -> metadata,
//                            stream -> attrs,
//                            stream -> language
//                            );

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
        qDeleteAll(chapters);
        qDeleteAll(programs);
    }

    operator QString() {
        return "lalka";
    }

    AttributeStream * dumpStream(AVFormatContext * context, int i, int is_output = 1) {
        char buf[256];
        int flags = (is_output ?
                         (context -> oformat ? context -> oformat -> flags : 0) :
                         (context -> iformat ? context -> iformat -> flags : 0)
                    );
        AVStream * st = context -> streams[i];

        char *separator = (char *)context -> dump_separator;
        char **codec_separator = (char **)av_opt_ptr(st -> codec -> av_class, st -> codec, "dump_separator");
        int use_format_separator = !*codec_separator;

        if (use_format_separator)
            *codec_separator = av_strdup(separator);

        avcodec_string(buf, sizeof(buf), st -> codec, is_output);
        AttributeStream * stream = new AttributeStream(i, QString("%1").arg(buf));

        if (use_format_separator)
            av_freep(codec_separator);

        if (flags & AVFMT_SHOW_IDS)
            stream -> attrs.insert("id", QString().sprintf("[0x%x]", st -> id));

        stream -> attrs.insert("time ratio", QString("%1/%2").arg(st -> time_base.num).arg(st -> time_base.den));
        stream -> attrs.insert("frames_count", QString("%1").arg(st -> codec_info_nb_frames));

        if (st -> sample_aspect_ratio.num && av_cmp_q(st -> sample_aspect_ratio, st -> codec -> sample_aspect_ratio)) {
            AVRational display_aspect_ratio;
            av_reduce(
               &display_aspect_ratio.num,
               &display_aspect_ratio.den,
               st -> codec -> width * st -> sample_aspect_ratio.num,
               st -> codec -> height * st -> sample_aspect_ratio.den,
               1024 * 1024
            );

            stream -> attrs.insert("aspect_ratio", QString("%1").arg(buf));

           av_log(NULL, AV_LOG_INFO, ", SAR %d:%d DAR %d:%d",
           st -> sample_aspect_ratio.num, st -> sample_aspect_ratio.den,
           display_aspect_ratio.num, display_aspect_ratio.den);
        }

        if (st -> codec -> codec_type == AVMEDIA_TYPE_VIDEO) {
            int fps = st -> avg_frame_rate.den && st -> avg_frame_rate.num;
            int tbr = st -> r_frame_rate.den && st -> r_frame_rate.num;
            int tbn = st -> time_base.den && st -> time_base.num;
            int tbc = st -> codec -> time_base.den && st -> codec -> time_base.num;

//            if (fps || tbr || tbn || tbc)
//                av_log(NULL, AV_LOG_INFO, "%s", separator);

            if (fps)
                stream -> attrs.insert("average_frame_rate", fpsStr(av_q2d(st -> avg_frame_rate), (tbr || tbn || tbc ? "fps, " : "fps")));
            if (tbr)
                stream -> attrs.insert("real_frame_rate", fpsStr(av_q2d(st -> r_frame_rate), (tbn || tbc ? "tbr, " : "tbr")));
            if (tbn)
                stream -> attrs.insert("time_base", fpsStr(1 / av_q2d(st -> time_base), (tbc ? "tbn, " : "tbn")));
            if (tbc)
                stream -> attrs.insert("codec_time_base", fpsStr(1 / av_q2d(st -> codec -> time_base), "tbc"));
        }

        QString style = "";

        if (st -> disposition & AV_DISPOSITION_DEFAULT)
            style += " default";

        if (st -> disposition & AV_DISPOSITION_DUB)
            style += " dub";

        if (st -> disposition & AV_DISPOSITION_ORIGINAL)
            style += " original";

        if (st -> disposition & AV_DISPOSITION_COMMENT)
            style += " comment";

        if (st -> disposition & AV_DISPOSITION_LYRICS)
            style += " lyrics";

        if (st -> disposition & AV_DISPOSITION_KARAOKE)
            style += " karaoke";

        if (st -> disposition & AV_DISPOSITION_FORCED)
            style += " forced";

        if (st -> disposition & AV_DISPOSITION_HEARING_IMPAIRED)
            style += " hearing impaired";

        if (st -> disposition & AV_DISPOSITION_VISUAL_IMPAIRED)
            style += " visual impaired";

        if (st -> disposition & AV_DISPOSITION_CLEAN_EFFECTS)
            style += " clean effects";

        stream -> attrs.insert("style", style.trimmed());

        dumpMetadata(
            st -> metadata,
            stream -> attrs,
            stream -> language
        );
//       dump_sidedata(NULL, st, " ");

       return stream;
    }

    QString fpsStr(double d, QString postfix) {
        uint64_t v = lrintf(d * 100);
        if (v % 100)
            return QString().sprintf("%3.2f ", d) + (postfix);
        else if (v % (100 * 1000))
            return QString().sprintf("%1.0f ", d) + (postfix);
        else
            return QString().sprintf("%1.0fk ", d / 1000) + (postfix);
    }

    void dumpMetadata(AVDictionary * dict, QHash<QString, QString> & data, QString & language) {
        AVDictionaryEntry * tag = 0;
        while ((tag = av_dict_get(dict, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            if (QString(tag -> key) == "language")
                language = tag -> value;
            else
                data.insert(tag -> key, tag -> value);
        }
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

    QList<AttributeChapter *> chapters;
    QList<AttributeProgram *> programs;

    bool hasAudio;
    bool hasVideo;
    bool hasSubtitles;
    bool hasAttachment;

    int is_output;
};

#endif // MEDIA_ATTRIBUTES_H
