#ifndef SETTINGS_H
#define SETTINGS_H

#define HOTKEY_NEXT 1
#define HOTKEY_NEXT_AND_DELETE 2
#define HOTKEY_PREV 3
#define HOTKEY_PLAY 4
#define HOTKEY_STOP 5


#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>

#include "model/hotkey_model/hotkey_model_item.h"

class Settings {
public:
    ~Settings() {
        delete hotkeys;
    }

    static Settings * instance();
    static void close() {
        delete self;
    }

    QString getDownloadPath() const;
    void setDownloadPath(QString newPath);

    QList<HotkeyModelItem *> * getHotKeys() const;
    void setHotKeys(QList<HotkeyModelItem *>);
    void setHotKeys(QJsonObject * hotkeysHash = 0);

private:
    Settings() {
        humanizeHotkeyText = QHash<int, QString>();
        humanizeHotkeyText.insert(HOTKEY_NEXT, "Activate next item");
        humanizeHotkeyText.insert(HOTKEY_NEXT_AND_DELETE, "Remove current and activate next");
        humanizeHotkeyText.insert(HOTKEY_PREV, "Activate prev item");
        humanizeHotkeyText.insert(HOTKEY_PLAY, "Play/pause");
        humanizeHotkeyText.insert(HOTKEY_STOP, "Stop");

        setHotKeys();
    }

    static Settings *self;

    QString downloadPath;
    QJsonObject * hotkeys;
    QHash<int, QString> humanizeHotkeyText;
};

#endif // SETTINGS_H
