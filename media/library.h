#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
#include <QTimer>
#include <Qthread>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
#include <QMutex>
#include <QApplication>
#include <QVector>

#include "genre.h"
#include "mediainfo.h"
#include "duration.h"

#include "model/model_item.h"

#include "misc/func_container.h"
class ModelItem;

class Library : public QObject {
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

    void clearRemote();
    void removeRemoteItem(ModelItem * item);
    void initItem(ModelItem * item, const QObject * caller, const char * slot);
    bool addItem(ModelItem * item, int state);
    void restoreItemState(ModelItem * item);

    QString filenameFilter(QString title);

    void setRemoteItemMax(int newMax);

private slots:
    void saveCatalogs();
    void startRemoteInfoProc();

private:
    static Library *self;

    Library() : QObject() {
        catalogs = new QHash<QChar,  QHash<QString, int>* >();
        catalogs_state = QHash<QChar, QList<QString> *>();
        catsSaveResult = QFuture<void>();

        timer = new QTimer();
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(saveCatalogs()));
        timer -> start(10000);

        QObject::connect(&remoteTimer, SIGNAL(timeout()), this, SLOT(startRemoteInfoProc()));
        remoteTimer.start(2000);

        QDir dir(libraryPath());
        if (!dir.exists()) {
            dir.mkpath(".");
        }
    }

    ~Library() {
        delete catalogs;
        delete timer;
    }

    ModelItem * procRemoteInfo(ModelItem * item);
    ModelItem * itemsInit(ModelItem * item);

    QString sitesFilter(QString title);
    QString forwardNumberPreFilter(QString title);
    QString spacesFilter(QString title);
    QString forwardNumberFilter(QString title);

    QString libraryPath();

    QString prepareName(QString gipoTitle, bool additional = false);
    bool proceedItemNames(QList<QString> * names, int state);

    QChar getCatalogChar(QString name);

    QHash<QString, int> * getCatalog(QChar letter);
    QHash<QString, int> * getCatalog(QString name);

    void initItemInfo(ModelItem * item);
//    QList<QString> * getNamesForObject(QString path, QString name);
//    QList<QString> * getNamesForItem(ModelItem * item);
//    QList<QString> * getNamesForItem(QString path);

    QHash<QString, int> * load(const QChar letter);
    void save();
    bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

    QHash<QChar, QHash<QString, int>* > * catalogs;
    QHash<QChar, QList<QString> *> catalogs_state;
    QHash<ModelItem *, FuncContainer> remote_collations;
    QList<ModelItem *> remote_items;
    int remote_items_max = 10;

    ModelItem * currRemote;
    QTimer * timer;
    QTimer remoteTimer;
    QMutex saveBlock;

    QFuture<void> catsSaveResult;
};

#endif // LIBRARY_H
