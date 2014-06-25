#ifndef API_PROCESS_H
#define API_PROCESS_H

#include <QObject>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

#include "misc/func_container.h"

//template <class T>
struct ApiFuncContainer {
    ApiFuncContainer() { }
    ApiFuncContainer(FuncContainer & container, QString & uid) {
        func = container;
        this -> uid = uid;
    }
    ~ApiFuncContainer() {}

    QString uid;
    FuncContainer func;
    QJsonObject result;
};


class ApiProcess : public QObject {
    Q_OBJECT   
public:
    ~ApiProcess() {
        qDeleteAll(processes);
        processes.clear();
    }

    static ApiProcess * instance();
    static void close() {
        delete self;
    }

    void start(QFuture<ApiFuncContainer *> future);

signals:
    void routineFinished(QJsonObject &);

protected slots:
//    void downloadConnectionResponsed();
//    void onTimer();
    void finished();

//signals:
//    void slotChanged(QString message);
//    void downloadProgress(ModelItem * item, int percentDone);
//    void downloadFinished(ModelItem * item, bool success);
//    void downloadError(ModelItem * item, QString message);

private:
    ApiProcess() {
    }

    QList<QFutureWatcher<ApiFuncContainer *> *> processes;
    static ApiProcess *self;
};

#endif // API_PROCESS_H
