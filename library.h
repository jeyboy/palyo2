#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QStringList>

class Library {
public:
    static Library * instance();
    static bool hasItem(const QString filename);
    static bool addItem(const QString filename);

private:
    Library() { catalogs = new QHash<QChar, QStringList>(); }

    static Library *self;
    static void load(const QChar letter);
    static void save(const QChar letter);

    QHash<QChar, QStringList> * catalogs;
};

#endif // LIBRARY_H
