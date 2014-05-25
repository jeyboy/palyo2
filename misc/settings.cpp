#include "settings.h"

Settings *Settings::self = 0;

Settings *Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

bool Settings::isShowInfo() const {
    return showInfo;
}
void Settings::setShowInfo(bool show) {
    showInfo = show;
}

bool Settings::isSpoilOnActivation() const {
    return spoilOnActivation;
}
void Settings::setSpoilOnActivation(bool show) {
    spoilOnActivation = show;
}

bool Settings::isMetricShow() const {
    return showMetric;
}
void Settings::setMetricShow(bool show) {
    showMetric = show;
}

bool Settings::isCheckboxShow() const {
    return showCheckbox;
}
void Settings::setCheckboxShow(bool show) {
    showCheckbox = show;
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

    foreach(QString key, hotkeys.keys()) {
        iKey = key.toInt();
        itemVals = QVector<QVariant>();
        itemVals.append(humanizeHotkeyText.value(iKey));
        itemVals.append(hotkeys.value(key));
        itemVals.append(iKey);
        ret -> append(new HotkeyModelItem(itemVals));
    }

    return ret;
}

QJsonObject Settings::hotkeysJson() const {
    return hotkeys;
}

void Settings::setHotKeys(QList<HotkeyModelItem *> hotkeyList) {
    foreach(HotkeyModelItem * item, hotkeyList) {
        hotkeys.insert(QString::number(item -> data(2).toInt()), QJsonValue::fromVariant(item -> data(1)));
    }
}

void Settings::setHotKeys(QJsonObject hotkeysHash) {
    if (hotkeysHash.empty()) {
        hotkeys = QJsonObject();
        hotkeys.insert(QString::number(HOTKEY_NEXT), QJsonValue::fromVariant("Ctrl+Down"));
        hotkeys.insert(QString::number(HOTKEY_NEXT_AND_DELETE), QJsonValue::fromVariant("Ctrl+Delete"));
        hotkeys.insert(QString::number(HOTKEY_PREV), QJsonValue::fromVariant("Ctrl+Up"));
        hotkeys.insert(QString::number(HOTKEY_PLAY), QJsonValue::fromVariant("Media Play"));
        hotkeys.insert(QString::number(HOTKEY_STOP), QJsonValue::fromVariant("Media Stop"));
    } else {
        hotkeys = hotkeysHash;
    }
}

void Settings::fromJson(QJsonObject settingsObj) {
    setHotKeys(settingsObj.value("hotkeys").toObject());
    downloadPath = settingsObj.value("download_path").toString(QCoreApplication::applicationDirPath() + "/downloads/");
    showCheckbox = settingsObj.value("show_checkboxes").toBool(true);
    showMetric = settingsObj.value("show_metric").toBool(true);
    spoilOnActivation = settingsObj.value("spoil_on_activation").toBool(true);
    showInfo = settingsObj.value("show_info").toBool(true);
}

QJsonObject Settings::toJson() {
    QJsonObject ret = QJsonObject();

    ret.insert("hotkeys", hotkeysJson());
    ret.insert("download_path", QJsonValue::fromVariant(downloadPath));
    ret.insert("show_checkboxes", QJsonValue::fromVariant(showCheckbox));
    ret.insert("show_metric", QJsonValue::fromVariant(showMetric));
    ret.insert("spoil_on_activation", QJsonValue::fromVariant(spoilOnActivation));
    ret.insert("show_info", QJsonValue::fromVariant(showInfo));

    return ret;
}