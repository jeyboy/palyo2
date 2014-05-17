#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H

#include <QObject>
#include "globalshortcut/qxtglobalshortcut.h"
#include "misc/hotkey_types.h"

class HotkeyManager : public QObject {
    Q_OBJECT
public:
    ~HotkeyManager() {
        delete next;
        delete next_and_delete;
        delete prev;
        delete play;
        delete stop;
    }

    static HotkeyManager * instance();

    static void close() {
        delete self;
    }

    bool registerSequence(int hotkeyType, QString sequence, QObject * receiver = 0, const char* slot = 0);

private:
    HotkeyManager() {
        next = 0;
        next_and_delete = 0;
        prev = 0;
        play = 0;
        stop = 0;
    }

    QxtGlobalShortcut * registerSequence(QObject * receiver, const char* slot);
    bool updateSequence(QxtGlobalShortcut * shortcut, QKeySequence sequence);

    static HotkeyManager * self;

    QxtGlobalShortcut * next;
    QxtGlobalShortcut * next_and_delete;
    QxtGlobalShortcut * prev;
    QxtGlobalShortcut * play;
    QxtGlobalShortcut * stop;
};

#endif // HOTKEY_MANAGER_H
