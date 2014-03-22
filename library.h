//TODO: improve singleton and add destructor

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>

#include "mediainfo.h"
#include "model_item.h"
#include "item_state.h"

class ModelItem;

class Library {
public:
    static Library * instance();
    static void close() {
        delete self;
    }

    void initItem(ModelItem * item);
    bool addItem(ModelItem * item, int state);

private:
    static Library *self;

    Library() {
        catalogs = new QHash<QChar,  QHash<QString, int> >();
        catalogs_state = QList<QChar>();
        items = QList<ModelItem *>();
        itemsInitResult = QFuture<void>();
    }

    ~Library() {
        delete catalogs;
    }

    void itemsInit();

    QString sitesFilter(QString title);
    QString forwardNumberPreFilter(QString title);
    QString spacesFilter(QString title);
    QString forwardNumberFilter(QString title);

    QString prepareName(QString gipoTitle, bool additional = false);
    bool proceedItemNames(QList<QString> * names, int state);

    QChar getCatalogChar(QString name);
//    QChar getCatalogChar(QChar l);

    QHash<QString, int> getCatalog(QChar letter);
    QHash<QString, int> getCatalog(QString name);

    QList<QString> * getNamesForObject(QString path, QString name);
    QList<QString> * getNamesForItem(ModelItem * item);
    QList<QString> * getNamesForItem(QString path);

    QHash<QString, int> load(const QChar letter);
    void save(const QChar letter);


    QHash<QChar, QHash<QString, int> > * catalogs;
    QList<QChar> catalogs_state;

    QList<ModelItem *> items;
    QFuture<void> itemsInitResult;
};

#endif // LIBRARY_H
