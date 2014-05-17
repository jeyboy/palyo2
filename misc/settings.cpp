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
    int iKey;

    foreach(QString key, hotkeys -> keys()) {
        iKey = key.toInt();
        itemVals = QVector<QVariant>();
        itemVals.append(humanizeHotkeyText.value(iKey));
        itemVals.append(hotkeys -> value(key));
        itemVals.append(iKey);
        ret -> append(new HotkeyModelItem(itemVals));
    }

    return ret;
}

void Settings::setHotKeys(QList<HotkeyModelItem *> hotkeyList) {
    foreach(HotkeyModelItem * item, hotkeyList) {
        hotkeys -> insert(QString::number(item -> data(2).toInt()), QJsonValue::fromVariant(item -> data(1)));
    }
}

void Settings::setHotKeys(QJsonObject * hotkeysHash) {
    if (hotkeysHash == 0) {
        hotkeys = new QJsonObject();
        hotkeys -> insert(QString::number(HOTKEY_NEXT), QJsonValue::fromVariant("Ctrl+Down"));
        hotkeys -> insert(QString::number(HOTKEY_NEXT_AND_DELETE), QJsonValue::fromVariant("Ctrl+Delete"));
        hotkeys -> insert(QString::number(HOTKEY_PREV), QJsonValue::fromVariant("Ctrl+Up"));
        hotkeys -> insert(QString::number(HOTKEY_PLAY), QJsonValue::fromVariant("Media Play"));
        hotkeys -> insert(QString::number(HOTKEY_STOP), QJsonValue::fromVariant("Media Stop"));
    } else {
        hotkeys = hotkeysHash;
    }
}
