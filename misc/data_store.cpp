#include "data_store.h"

DataStore::DataStore(QString name) {
    filename = name;
    state = load();
}
DataStore::~DataStore() {
//    save();
}

bool DataStore::load() {
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = qUncompress(loadFile.readAll());
<<<<<<< HEAD
//    QByteArray saveData = loadFile.readAll();
=======
>>>>>>> ce35fa42eb2bef9ee877b6a11148236a9e77ba63
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    json = loadDoc.object();

    return true;
}
bool DataStore::save() {
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument saveDoc(json);
    saveFile.write(qCompress(saveDoc.toJson(), 9));

    return true;
}

void DataStore::clear() {
    json = QJsonObject();
}

QStringList DataStore::keys() {
    return json.keys();
}

QJsonValueRef DataStore::read(QString key) {
    return json[key];
}

void DataStore::write(QString key, double value) {
    json[key] = value;
}

void DataStore::write(QString key, QJsonArray value) {
    json[key] = value;
}
void DataStore::write(QString key, QJsonObject value) {
    json[key] = value;
}
void DataStore::write(QString key, QString value) {
    json[key] = value;
}
void DataStore::write(QString key, int value) {
    json[key] = value;
}
void DataStore::append(QString key, QString subkey, QString value) {
    QJsonObject subObj;
    subObj[subkey] = value;
    json[key].toArray().append(subObj);
}
