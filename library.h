//TODO: improve singleton and add destructor

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>

#include "mediainfo.h"
#include "model_item.h"
#include "model_item_state.h"
#include "library_item.h"

class ModelItem;
class LibraryItem;

class Library : QObject {
    Q_OBJECT
public:
    static Library * instance();
    static void close() {
        if (self != 0) {
            self -> timer -> stop();
            self -> save();
            delete self;
        }
    }

    void initItem(LibraryItem * item);
    bool addItem(ModelItem * item, int state);
    void restoreItemState(LibraryItem * item);

private slots:
    void saveCatalogs();

private:
    static Library *self;

    Library() : QObject() {
        catalogs = new QHash<QChar,  QHash<QString, int>* >();
        catalogs_state = QHash<QChar, QList<QString> *>();
        items = QList<LibraryItem *>();
        itemsInitResult = QFuture<void>();
        catsSaveResult = QFuture<void>();

        timer = new QTimer();
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(saveCatalogs()));
        timer -> start(10000);

        QDir dir(libraryPath());
        if (!dir.exists()) {
            dir.mkpath(".");
        }
    }

    ~Library() {
        delete catalogs;
        delete timer;
    }

    void itemsInit();

    QString sitesFilter(QString title);
    QString forwardNumberPreFilter(QString title);
    QString spacesFilter(QString title);
    QString forwardNumberFilter(QString title);

    QString libraryPath();

    QString prepareName(QString gipoTitle, bool additional = false);
    bool proceedItemNames(QList<QString> * names, int state);

    QChar getCatalogChar(QString name);
//    QChar getCatalogChar(QChar l);

    QHash<QString, int> * getCatalog(QChar letter);
    QHash<QString, int> * getCatalog(QString name);

    QList<QString> * getNamesForObject(QString path, QString name);
    QList<QString> * getNamesForItem(ModelItem * item);
    QList<QString> * getNamesForItem(QString path);

    QHash<QString, int> * load(const QChar letter);
    void save();

    QHash<QChar, QHash<QString, int>* > * catalogs;
    QHash<QChar, QList<QString> *> catalogs_state;

    QList<LibraryItem *> items;
    QTimer *timer;

    QFuture<void> itemsInitResult;
    QFuture<void> catsSaveResult;
};

#endif // LIBRARY_H
