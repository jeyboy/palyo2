#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
#include <QThread>
#include <QApplication>

class Library {
public:
    static Library * instance();
    static bool addItem(const QString filename, int state);
    static int getItemState(const QString filename);
    static void setItemState(const QString filename, int state);

private:
    Library() { catalogs = new QHash<QChar,  QHash<QString, int> >(); }

    static Library *self;
    static QChar getCatalogName(QChar l);
    static QHash<QString, int> getCatalog(QString name);
    static QHash<QString, int> load(const QChar letter);
    static void save(const QChar letter);

    QHash<QChar, QHash<QString, int> > * catalogs;
};

#endif // LIBRARY_H
