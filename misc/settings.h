#ifndef SETTINGS_H
#define SETTINGS_H

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
        setHotKeys();
    }

    static Settings *self;

    QString downloadPath;
    QJsonObject * hotkeys;
};

#endif // SETTINGS_H
