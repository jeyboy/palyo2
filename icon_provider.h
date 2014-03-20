//TODO: improve singleton and add destructor

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

    static IconProvider * instance();
    static QIcon fileIcon(const QString filename, QString extension);
    static QIcon dirIcon();

private:
    IconProvider() { icons = new QHash<QString, QIcon>(); }

    static IconProvider *self;

    QHash<QString, QIcon> * icons;
    QFileIconProvider iconProvider;
};

#endif // ICON_PROVIDER_H
