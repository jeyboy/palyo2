#ifndef TEU_H
#define TEU_H

#include <QString>
#include <QJsonObject>

class TeuAuth {
public:
    TeuAuth();
    TeuAuth(QJsonObject & hash);

    void setParams(QString accessToken, QString userID, QString expiresIn);

    QString getToken();
    QString getExpire();
    QString getUserID();

    void fromJson(QJsonObject & hash);
    QJsonObject & toJson(QJsonObject & root);

private:
    QString token;
    QString expires_in;
    QString user_id;
};

#endif // TEU_H
