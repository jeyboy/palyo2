#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <QHash>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include "misc/data_store.h"

class Extensions {
public:
    ~Extensions() {
        ext -> clear();
        ext -> save();
    }

    static Extensions * instance();
    static void close() {
        delete self;
    }

    QFileInfoList folderFiles(QFileInfo file);
    QFileInfoList folderDirectories(QFileInfo file);
    QStringList & activeFilterList() const;
    QStringList & filterList(QString & name) const;
    inline QString & activeFilterName() const { return activeFilter; }
    inline QStringList & presetsList() const { return QStringList(filters.keys()); }

private:
    Extensions() {
        ext = new DataStore("extensions.json");

        if (ext -> state) {
            activeFilter = ext -> read("active").toString("all");
            filters = ext -> read("filters").toVariant().value<QHash<QString, QStringList> >();
        } else {
            QStringList commonfiltersList;
            commonfiltersList << "*.*";
            filters.insert("all", commonfiltersList);

            QStringList filtersList;
            filtersList << "*.wav"
                        << "*.aiff"
                        << "*.aif"
                        << "*.mp3"
                        << "*.mp2"
                        << "*.mp1"
                        << "*.ogg"
                        << "*.wma"
                        << "*.mpc"
                        << "*.aac"
                        << "*.alac"
                        << "*.ac3"
                        << "*.wv"
                        << "*.ape"
                        << "*.flac";

            filters.insert("audio", filtersList);
            activeFilter = "all";
        }
    }

    static Extensions *self;

    QString activeFilter;
    QHash<QString, QStringList> filters;
    DataStore * ext;
};

#endif // EXTENSIONS_H
