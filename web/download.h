#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

#include "override/custom_network_access_manager.h"

struct DownloadPosition {
    DownloadPosition(QObject * _caller, const char * _slot, QUrl _uri, QUrl _savePath) {
        caller = _caller;
        slot = _slot;
        uri = _uri;
        savePath = _savePath;
    }

    QObject * caller;
    const char * slot;
    QUrl uri;
    QUrl savePath;
};

class Download : public QObject {
    Q_OBJECT   
public:
    Download();
    ~Download();

    QString getError();

    void start(QObject * caller, const char * slot, QUrl uri, QUrl savePath);

    CustomNetworkAccessManager * manager() const;

protected slots:
    void downloadConnectionResponsed();

signals:
    void downloadProgress(void * item, int percentDone);
    void downloadFinished(void * item, bool success);
    void downloadError(void * item, QString message);

protected:
    CustomNetworkAccessManager * netManager;

    QString error;

    QHash<void *, DownloadPosition *> * downloads;
};

#endif // DOWNLOAD_H
