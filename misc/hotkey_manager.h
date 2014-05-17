#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H

#include <QObject>
#include "globalshortcut/qxtglobalshortcut.h"

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

    bool registerNext(QString sequence, QObject * receiver, const char* slot);
    bool registerNextAndDelete(QString sequence, QObject * receiver, const char* slot);
    bool registerPrev(QString sequence, QObject * receiver, const char* slot);
    bool registerPlay(QString sequence, QObject * receiver, const char* slot);
    bool registerStop(QString sequence, QObject * receiver, const char* slot);

private:
    HotkeyManager() {
        next = 0;
        next_and_delete = 0;
        prev = 0;
        play = 0;
        stop = 0;
    }

    bool registerSequence(QxtGlobalShortcut * shortcut, QString sequence, QObject * receiver, const char * slot);

    static HotkeyManager * self;

    QxtGlobalShortcut * next;
    QxtGlobalShortcut * next_and_delete;
    QxtGlobalShortcut * prev;
    QxtGlobalShortcut * play;
    QxtGlobalShortcut * stop;
};

#endif // HOTKEY_MANAGER_H
