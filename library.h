//TODO: improve singleton and add destructor

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>

#include "model_item.h"
#include "item_state.h"

class ModelItem;

class Library {
public:
    static Library * instance();

    static void initItem(ModelItem * item);
    static bool proceedItem(const QString filename, int state, bool last = true);
    static bool addItem(const QString filename, int state = STATE_LISTENED);
    static int getItemState(const QString filename);
    static void setItemState(const QString filename, int state);
    static QString prepareName(QString gipoTitle);

    static QList<QString> * getNamesForItem(ModelItem * item);

private:
    void itemsInit();

    static QString sitesFilter(QString title);
    static QString forwardNumberPreFilter(QString title);
    static QString spacesFilter(QString title);
    static QString forwardNumberFilter(QString title);

    Library() {
        catalogs = new QHash<QChar,  QHash<QString, int> >();
        catalogs_state = QList<QChar>();
        items = QList<ModelItem *>();
        itemsInitResult = QFuture<void>();
    }

    static Library *self;
    static QChar getCatalogName(QChar l);
    static QHash<QString, int> getCatalog(QString name);
    static QHash<QString, int> load(const QChar letter);
    static void save(const QChar letter);

    QHash<QChar, QHash<QString, int> > * catalogs;
    QList<QChar> catalogs_state;

    QList<ModelItem *> items;
    QFuture<void> itemsInitResult;
};

#endif // LIBRARY_H
