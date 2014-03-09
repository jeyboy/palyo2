#ifndef DATA_STORE_H
#define DATA_STORE_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QStringList>

class DataStore {
public:
    DataStore(QString name);
    ~DataStore();

    bool load();
    bool save();

    QJsonValueRef read(QString key);
    void write(QString key, double value);
    void write(QString key, QJsonArray value);
    void write(QString key, QJsonObject value);
    void write(QString key, QString value);
    void write(QString key, int value);
    void append(QString key, QString subkey, QString value);
    void clear();
    QStringList keys();

    bool state;
private:
    QString filename;
    QJsonObject json;
};

#endif // DATA_STORE_H
