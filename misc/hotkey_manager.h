#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H

#include <QObject>
#include "globalshortcut/qxtglobalshortcut.h"

class HotkeyManager : public QObject {
    Q_OBJECT
public:
    ~HotkeyManager() {

    }

    static HotkeyManager * instance();

    static void close() {
        delete self;
    }

    bool registerNext(QString sequence, QObject * receiver, char* slot);
    bool registerNextAndDelete(QString sequence, QObject * receiver, char* slot);
    bool registerPrev(QString sequence, QObject * receiver, char* slot);
    bool registerPlay(QString sequence, QObject * receiver, char* slot);
    bool registerStop(QString sequence, QObject * receiver, char* slot);

private:
    HotkeyManager() {

    }

    static HotkeyManager * self;

    QxtGlobalShortcut * next;
    QxtGlobalShortcut * next_and_delete;
    QxtGlobalShortcut * prev;
    QxtGlobalShortcut * play;
    QxtGlobalShortcut * stop;
};

#endif // HOTKEY_MANAGER_H
