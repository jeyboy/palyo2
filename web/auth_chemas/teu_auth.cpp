#include "teu_auth.h"

TeuAuth::TeuAuth() {}
TeuAuth::TeuAuth(QJsonObject & hash) {
    fromJson(hash);
}

void TeuAuth::setParams(QString accessToken, QString userID, QString expiresIn) {
    token = accessToken;
    user_id = userID;
    expires_in = expiresIn;
}

QString TeuAuth::getToken() {
    return token;
}
QString TeuAuth::getExpire() {
    return expires_in;
}
QString TeuAuth::getUserID() {
    return user_id;
}

void TeuAuth::fromJson(QJsonObject & hash) {
    user_id = hash.value("_u_").toString();
    token = hash.value("_t_").toString();
    expires_in = hash.value("_e_").toString();
}
QJsonObject & TeuAuth::toJson(QJsonObject & root) {
    root["_u_"] = getUserID();
    root["_t_"] = getToken();
    root["_e_"] = getExpire();

    return root;
}
