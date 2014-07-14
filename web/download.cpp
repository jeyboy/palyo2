#include "download.h"

Download *Download::self = 0;

Download *Download::instance() {
    if(!self)
        self = new Download();
    return self;
}

QString Download::getError() {
    return error;
}

void Download::onTimer() {
    if (isReady) {
        if (!queue -> isEmpty()) {
            isReady = false;

            ModelItem * item = queue -> takeFirst();
            item -> setDownloadProgress(0);
            QNetworkReply * m_http = manager() -> get(QNetworkRequest(item -> toUrl()));
            DownloadPosition * pos = downloads -> take(item);
            downloads -> insert(m_http, pos);
            QObject::connect(m_http, SIGNAL(finished()), this, SLOT(downloadConnectionResponsed()));
            emit slotChanged("(Downloads remain: " + QString::number(queue -> count()) + ") " + item -> data(TITLEID).toString());
        }
    }
}

void Download::finishDownload() {
    emit slotChanged("(O_o)");
//    QFutureWatcher<QNetworkReply *> * curr = (QFutureWatcher<QNetworkReply *>)sender();
    delete downloader -> result();
    isReady = true;
    delete downloader;
    downloader = 0;
}

void Download::start(Model * model, ModelItem * item, QUrl savePath) {
    queue -> append(item);
    downloads -> insert(item, new DownloadPosition(model, item, savePath));
}

void Download::downloadConnectionResponsed() {
    QNetworkReply * reply = (QNetworkReply*)QObject::sender();
    QVariant possibleRedirectUrl = reply -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        DownloadPosition * pos = downloads -> take(reply);
        reply -> close();
        delete reply;

        QNetworkReply * m_http = manager() -> get(QNetworkRequest(possibleRedirectUrl.toUrl()));
        downloads -> insert(m_http, pos);
        QObject::connect(m_http, SIGNAL(finished()), this, SLOT(downloadConnectionResponsed()));
    } else {
        downloader = new QFutureWatcher<QNetworkReply *>();
        connect(downloader, SIGNAL(finished()), this, SLOT(finishDownload()));
        downloader -> setFuture(QtConcurrent::run(this, &Download::downloading, reply));
    }
}


QNetworkReply * Download::downloading(QNetworkReply * reply) {
    DownloadPosition * position = downloads -> value(reply);
    QObject::connect(this, SIGNAL(downloadFinished(ModelItem *, bool)), position -> model, SLOT(itemDownloadFinished(ModelItem *, bool)));
    QObject::connect(this, SIGNAL(downloadProgress(ModelItem *, int)), position -> model, SLOT(itemDownloadProgress(ModelItem *, int)));

    QFile file(position -> savePath.toLocalFile());

//    QIODevice::Append | QIODevice::Unbuffered

    if (!file.open(QIODevice::WriteOnly)) {
        emit downloadError(position -> item, file.errorString());
        emit downloadFinished(position -> item, false);
    } else {
        if (!file.resize(reply -> bytesAvailable())) {
            emit downloadError(position -> item, file.errorString());
            emit downloadFinished(position -> item, false);
        } else {
            QByteArray buffer;
            qint64 pos = 0;
            double limit = reply -> bytesAvailable();
            int bufferLength = 1024 * 1024 * 1; //1 mb

            while(!reply -> atEnd()) {
                try {
                    buffer = reply -> read(bufferLength);
                    pos += buffer.length();
                    file.write(buffer);

                    progress = (pos/limit) * 100;
                    emit downloadProgress(position -> item, progress);
                }

                catch(...) {
                    emit downloadError(position -> item, "Some error occured while download...");
                    emit downloadFinished(position -> item, false);
                }
            }

            file.close();
            reply -> close();
            emit downloadFinished(position -> item, true);
        }
    }


    QObject::disconnect(this, SIGNAL(downloadFinished(ModelItem *, bool)), position -> model, SLOT(itemDownloadFinished(ModelItem *, bool)));
    QObject::disconnect(this, SIGNAL(downloadProgress(ModelItem *, int)), position -> model, SLOT(itemDownloadProgress(ModelItem *, int)));
    return reply;
}

CustomNetworkAccessManager * Download::manager() const {
    return netManager;
}

int Download::getProgress() const {
    return progress;
}

int Download::getQueueLength() const {
    return queue -> count();
}
