#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QIcon>
#include <QHash>
#include <QPixmapCache>
#include <QFileIconProvider>
#include <QStyle>
#include <QApplication>

class IconProvider {
public:
    ~IconProvider() {
        delete icons;
    }

    static IconProvider * instance();
    static QIcon fileIcon(const QString filename, QString extension);
    static QIcon dirIcon();
    static QIcon remoteIcon();
    static QIcon missedIcon();
    static void close() {
        delete self;
    }

private:
    IconProvider() {
        icons = new QHash<QString, QIcon>();

        icons -> insert("mp3", QIcon(":/ext/_mp3"));
        icons -> insert("aac", QIcon(":/ext/_aac"));
        icons -> insert("ape", QIcon(":/ext/_ape"));
        icons -> insert("flac", QIcon(":/ext/_flac"));
        icons -> insert("m4a", QIcon(":/ext/_m4a"));
        icons -> insert("mod", QIcon(":/ext/_mod"));
        icons -> insert("mpc", QIcon(":/ext/_mpc"));
        icons -> insert("ogg", QIcon(":/ext/_ogg"));
        icons -> insert("wav", QIcon(":/ext/_wav"));
        icons -> insert("wma", QIcon(":/ext/_wma"));
    }

    static IconProvider *self;

    QHash<QString, QIcon> * icons;
    QFileIconProvider iconProvider;
};

#endif // ICON_PROVIDER_H
