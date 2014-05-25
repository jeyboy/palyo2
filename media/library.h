#ifndef LIBRARY_H
#define LIBRARY_H

#include <QHash>
#include <QFile>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
#include <QMutex>
#include <QApplication>
#include <QVector>

#include "genre.h"
#include "mediainfo.h"
#include "duration.h"

#include "model/model_item.h"
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

    void initItem(ModelItem * item, const QObject * caller, char * slot);
    bool addItem(ModelItem * item, int state);
    void restoreItemState(ModelItem * item);

    QString filenameFilter(QString title);

private slots:
    void saveCatalogs();

private:
    static Library *self;

    Library() : QObject() {
        catalogs = new QHash<QChar,  QHash<QString, int>* >();
        catalogs_state = QHash<QChar, QList<QString> *>();
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

    ModelItem * itemsInit(ModelItem * item);

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

    void initItemInfo(ModelItem * item);
//    QList<QString> * getNamesForObject(QString path, QString name);
//    QList<QString> * getNamesForItem(ModelItem * item);
//    QList<QString> * getNamesForItem(QString path);

    QHash<QString, int> * load(const QChar letter);
    void save();
    bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

    QHash<QChar, QHash<QString, int>* > * catalogs;
    QHash<QChar, QList<QString> *> catalogs_state;

    QTimer *timer;
    QMutex saveBlock;

    QFuture<void> catsSaveResult;
};

#endif // LIBRARY_H
