#include "api_process.h"

ApiProcess *ApiProcess::self = 0;

ApiProcess *ApiProcess::instance() {
    if(!self)
        self = new ApiProcess();
    return self;
}

void ApiProcess::finished() {
    QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *) sender();
    ApiFuncContainer * func = initiator -> result();
    qDebug() << "_________ FINISHED";
    connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> func.obj, func -> func.slot);
    qDebug() << "_________ FINISHED";
    emit routineFinished(func -> result);
    qDebug() << "_________ FINISHED";
    disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> func.obj, func -> func.slot);
    qDebug() << "_________ FINISHED";
    processes.removeOne(initiator);
}

void ApiProcess::start(QFuture<ApiFuncContainer *> future) {
    QFutureWatcher<ApiFuncContainer *> * initiator = new QFutureWatcher<ApiFuncContainer *>();
    processes.append(initiator);
    connect(initiator, SIGNAL(finished()), this, SLOT(finished()));
    initiator -> setFuture(future);
}
