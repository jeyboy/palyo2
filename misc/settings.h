#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QApplication>

#include "model/hotkey_model/hotkey_model_item.h"
#include "misc/hotkey_types.h"

class Settings {
public:
    ~Settings() {

    }

    static Settings * instance();
    static void close() {
        delete self;
    }

    bool getCheckboxShow() const;
    void setCheckboxShow(bool show);

    QString getDownloadPath() const;
    void setDownloadPath(QString newPath);

    QList<HotkeyModelItem *> * getHotKeys() const;
    void setHotKeys(QList<HotkeyModelItem *>);

    void fromJson(QJsonObject settingsObj = QJsonObject());
    QJsonObject toJson();

private:
    Settings() {
        humanizeHotkeyText = QHash<int, QString>();
        humanizeHotkeyText.insert(HOTKEY_NEXT, "Activate next item");
        humanizeHotkeyText.insert(HOTKEY_NEXT_AND_DELETE, "Remove current and activate next");
        humanizeHotkeyText.insert(HOTKEY_PREV, "Activate prev item");
        humanizeHotkeyText.insert(HOTKEY_PLAY, "Play/pause");
        humanizeHotkeyText.insert(HOTKEY_STOP, "Stop");

//        setHotKeys();
    }

    void setHotKeys(QJsonObject hotkeysHash = QJsonObject());
    QJsonObject hotkeysJson() const;

    static Settings *self;

    QString downloadPath;
    bool showCheckbox;
    QJsonObject hotkeys;
    QHash<int, QString> humanizeHotkeyText;
};

#endif // SETTINGS_H
