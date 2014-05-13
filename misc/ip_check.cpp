#include "ip_check.h"

QString IpCheck::getIp() {
    QTcpSocket dnsTestSocket;
    QString localIP = "127.0.0.1";    //fall back
    QString googleDns = "8.8.8.83";  //try google DNS or sth. else reliable first
    dnsTestSocket.connectToHost(googleDns, 53);
    if (dnsTestSocket.waitForConnected(3000)) {
        localIP = dnsTestSocket.localAddress().toString();
    } else {
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            QString guessedGatewayAddress = address.toString().section(".", 0, 2) + ".1";
            qDebug() << address.toString() << " : " << address.toIPv4Address();

            if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost)) {
                dnsTestSocket.connectToHost(guessedGatewayAddress, 53);
                if (dnsTestSocket.waitForConnected(3000)) {
                    localIP = dnsTestSocket.localAddress().toString();
                    break;
                }
            }
        }
    }

    return localIP;
}
