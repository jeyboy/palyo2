#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

#include "model/model.h"
#include "model/model_item.h"
#include "override/custom_network_access_manager.h"

struct DownloadPosition {
    DownloadPosition(Model * _model, ModelItem * _item, QUrl _savePath) {
        item = _item;
        savePath = _savePath;
        model = _model;
    }

    Model * model;
    ModelItem * item;
    QUrl savePath;
};

class Download : public QObject {
    Q_OBJECT   
public:
    ~Download() {
        delete netManager;
        qDeleteAll(downloads -> values());
        delete downloads;
        delete queue;
    }

    static Download * instance();
    QString getError();

    void start(Model * model, ModelItem * item, QUrl savePath);

    CustomNetworkAccessManager * manager() const;

    static void close() {
        delete self;
    }

    int getProgress() const;
    int getQueueLength() const;

protected slots:
    void downloadConnectionResponsed();
    void onTimer();
    void finishDownload();

signals:
    void downloadProgress(ModelItem * item, int percentDone);
    void downloadFinished(ModelItem * item, bool success);
    void downloadError(ModelItem * item, QString message);

protected:
    QNetworkReply * downloading(QNetworkReply * reply);

    CustomNetworkAccessManager * netManager;

    QString error;

    QHash<void *, DownloadPosition *> * downloads;
    QList<ModelItem *> * queue;

private:
    Download() {
        netManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
        downloads = new QHash<void *, DownloadPosition *>();
        queue = new QList<ModelItem *>();

        QObject::connect(&remoteTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
        remoteTimer.start(1000);
        progress = 0;
        downloader = 0;
        isReady = true;
    }

    QFutureWatcher<QNetworkReply *> * downloader;
    static Download *self;
    QTimer remoteTimer;
    int progress;
    bool isReady;
};

#endif // DOWNLOAD_H
