#include "settings.h"

Settings *Settings::self = 0;

Settings *Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

QString Settings::getDownloadPath() const {
    return downloadPath;
}

void Settings::setDownloadPath(QString newPath) {
    downloadPath = newPath;
}

QList<HotkeyModelItem *> * Settings::getHotKeys() const {
    QVector<QVariant> itemVals;
    QList<HotkeyModelItem *> * ret = new QList<HotkeyModelItem *>();

    foreach(QString key, hotkeys -> keys()) {
        itemVals = QVector<QVariant>();
        itemVals.append(key);
        itemVals.append(hotkeys -> value(key));
        ret -> append(new HotkeyModelItem(itemVals));
    }

    return ret;
}

void Settings::setHotKeys(QList<HotkeyModelItem *> hotkeyList) {
    foreach(HotkeyModelItem * item, hotkeyList) {
        hotkeys -> insert(item -> data(0).toString(), QJsonValue::fromVariant(item -> data(1)));
    }
}

void Settings::setHotKeys(QJsonObject * hotkeysHash) {
    if (hotkeysHash == 0) {
        hotkeys = new QJsonObject();
        hotkeys -> insert("Activate next item", QJsonValue::fromVariant("Ctrl+Down"));
        hotkeys -> insert("Remove current and activate next", QJsonValue::fromVariant("Ctrl+Delete"));
        hotkeys -> insert("Activate prev item", QJsonValue::fromVariant("Ctrl+Up"));
    } else {
        hotkeys = hotkeysHash;
    }
}
