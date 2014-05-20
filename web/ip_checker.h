#ifndef IP_CHECKER_H
#define IP_CHECKER_H

#include <QObject>
#include <QUrl>
#include "override/custom_network_access_manager.h"

class IpChecker : public QObject {
    Q_OBJECT   
public:
    static IpChecker * instance();
    static IpChecker * instance(QString currIp);
    static void close() {
        delete self;
    }

    ~IpChecker() {

    }

    bool isLocalOnly() const;
    bool isInitialized() const;
    bool isProccessed() const;

    QString currentIp() const;

protected slots:
    void ipResponse();

signals:
    void ipChanged();

protected:
    void initIp();
    bool hasIp();

    IpChecker(QString currIp = "") : QObject() {
        ip = currIp;
        initialized = !ip.isEmpty();
        noInternet = false;
        inProgress = false;
        netManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);

        if (!initialized || (initialized && !hasIp()))
            initIp();
    }

    static IpChecker *self;

    CustomNetworkAccessManager * netManager;

    QString ip;
    bool noInternet, inProgress, initialized;
};

#endif // IP_CHECKER_H
