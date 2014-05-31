#include "settings.h"

Settings *Settings::self = 0;

Settings *Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

QBrush Settings::buildGradient(QRect rect, QColor color, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    grad.setColorAt(0, color);
    if (dark)
        grad.setColorAt(1, QColor::fromRgba(qRgba(0, 0, 0, 192)));
    else
        grad.setColorAt(1, Qt::white);

    return grad;
}

QBrush Settings::defaultState(QRect rect, bool dark) {
    return buildGradient(rect, defaultColor1, dark);
}
QBrush Settings::listenedState(QRect rect, bool dark) {
    return buildGradient(rect, listenedColor1, dark);
}
QBrush Settings::likedState(QRect rect, bool dark) {
    return buildGradient(rect, likedColor1, dark);
}
QBrush Settings::playedState(QRect rect, bool dark) {
    return buildGradient(rect, playedColor1, dark);
}

QBrush Settings::unprocessedState(QRect rect, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    if (dark) {
        grad.setColorAt(0, folderColor1);
        grad.setColorAt(0.8, Qt::black);
    } else
        grad.setColorAt(0, Qt::white);

    return grad;
}


bool Settings::isUseGradient() const {
    return useGradient;
}
void Settings::setUseGradient(bool use) {
    useGradient = use;
}

QColor Settings::getDefaultColor() const {
    return defaultColor1;
}
void Settings::setDefaultColor(QColor newColor) {
    defaultColor1 = newColor;
}

QColor Settings::getListenedColor() const {
    return listenedColor1;
}
void Settings::setListenedColor(QColor newColor) {
    listenedColor1 = newColor;
}

QColor Settings::getLikedColor() const {
    return likedColor1;
}
void Settings::setLikedColor(QColor newColor) {
    likedColor1 = newColor;
}

QColor Settings::getPlayedColor() const {
    return playedColor1;
}
void Settings::setPlayedColor(QColor newColor) {
    playedColor1 = newColor;
}

QColor Settings::getFolderColor() const {
    return folderColor1;
}
void Settings::setFolderColor(QColor newColor) {
    folderColor1 = newColor;
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
        hotkeys.insert(QString::number(HOTKEY_SETTINGS), QJsonValue::fromVariant("Ctrl+Q"));

        hotkeys.insert(QString::number(HOTKEY_POS_SLIDE_FORWARD), QJsonValue::fromVariant("Ctrl+]"));
        hotkeys.insert(QString::number(HOTKEY_POS_SLIDE_BACKWARD), QJsonValue::fromVariant("Ctrl+["));
        hotkeys.insert(QString::number(HOTKEY_VOL_SLIDE_FORWARD), QJsonValue::fromVariant("Ctrl+}"));
        hotkeys.insert(QString::number(HOTKEY_VOL_SLIDE_BACKWARD), QJsonValue::fromVariant("Ctrl+{"));
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
    useGradient = settingsObj.value("use_gradient").toBool(true);

    QVariant defaultColor1Var = settingsObj.value("default_color1").toVariant();
    defaultColor1 = defaultColor1Var.isValid() ? defaultColor1Var.value<QColor>() : QColor(98, 173, 248);

    QVariant listenedColor1Var = settingsObj.value("listened_color1").toVariant();
    listenedColor1 = listenedColor1Var.isValid() ? listenedColor1Var.value<QColor>() : QColor(240, 128, 128);

    QVariant likedColor1Var = settingsObj.value("liked_color1").toVariant();
    likedColor1 = likedColor1Var.isValid() ? likedColor1Var.value<QColor>() : QColor(232, 196, 0);

    QVariant playedColor1Var = settingsObj.value("played_color1").toVariant();
    playedColor1 = playedColor1Var.isValid() ? playedColor1Var.value<QColor>() : QColor(144, 238, 144);

    QVariant folderColor1Var = settingsObj.value("folder_color1").toVariant();
    folderColor1 = folderColor1Var.isValid() ? folderColor1Var.value<QColor>() : QColor(128, 128, 128, 92);
}

QJsonObject Settings::toJson() {
    QJsonObject ret = QJsonObject();

    ret.insert("hotkeys", hotkeysJson());
    ret.insert("download_path", QJsonValue::fromVariant(downloadPath));
    ret.insert("show_checkboxes", QJsonValue::fromVariant(showCheckbox));
    ret.insert("show_metric", QJsonValue::fromVariant(showMetric));
    ret.insert("spoil_on_activation", QJsonValue::fromVariant(spoilOnActivation));
    ret.insert("show_info", QJsonValue::fromVariant(showInfo));

    ret.insert("default_color1", QJsonValue::fromVariant(defaultColor1));
    ret.insert("listened_color1", QJsonValue::fromVariant(listenedColor1));
    ret.insert("liked_color1", QJsonValue::fromVariant(likedColor1));
    ret.insert("played_color1", QJsonValue::fromVariant(playedColor1));
    ret.insert("folder_color1", QJsonValue::fromVariant(folderColor1));

    return ret;
}
