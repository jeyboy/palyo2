//TODO: improve singleton and add destructor

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
//#include <QThread>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>

#include "mediainfo.h"
#include "model_item.h"
#include "item_state.h"

class ModelItem;

class Library : QObject {
    Q_OBJECT
public:
    static Library * instance();
    static void close() {
        self -> timer -> stop();
        self -> save();
        delete self;
    }

    void initItem(ModelItem * item);
    bool addItem(ModelItem * item, int state);
    void restoreItemState(ModelItem * item);  

private slots:
    void saveCatalogs();

private:
    static Library *self;

    Library() : QObject() {
        catalogs = new QHash<QChar,  QHash<QString, int>* >();
        catalogs_state = QList<QChar>();
        items = QList<ModelItem *>();
        itemsInitResult = QFuture<void>();
        catsSaveResult = QFuture<void>();

        timer = new QTimer();
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(saveCatalogs()));
        timer -> start(10000);
        //TODO: create library folder if it does not exist
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
    QList<QChar> catalogs_state;

    QList<ModelItem *> items;
    QTimer *timer;

    QFuture<void> itemsInitResult;
    QFuture<void> catsSaveResult;
};

#endif // LIBRARY_H
