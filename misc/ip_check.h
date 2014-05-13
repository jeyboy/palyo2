#ifndef IP_CHECK_H
#define IP_CHECK_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>

class IpCheck {
public:
    static QString getIp();
};

#endif // IP_CHECK_H
