#include "hotkey_manager.h"

HotkeyManager *HotkeyManager::self = 0;

HotkeyManager *HotkeyManager::instance() {
    if(!self)
        self = new HotkeyManager();
    return self;
}

bool HotkeyManager::registerSequence(QxtGlobalShortcut * shortcut, QString sequence, QObject * receiver, const char* slot) {
    shortcut = new QxtGlobalShortcut(QKeySequence(sequence));
    if (shortcut -> isEnabled()) {
        connect(shortcut, SIGNAL(activated()), receiver, slot);
        return true;
    } else {
        return false;
    }
}

bool HotkeyManager::registerNext(QString sequence, QObject * receiver, const char* slot) {
    return registerSequence(next, sequence, receiver, slot);
}

bool HotkeyManager::registerNextAndDelete(QString sequence, QObject * receiver, const char* slot) {
    return registerSequence(next_and_delete, sequence, receiver, slot);
}

bool HotkeyManager::registerPrev(QString sequence, QObject * receiver, const char* slot) {
    return registerSequence(prev, sequence, receiver, slot);
}

bool HotkeyManager::registerPlay(QString sequence, QObject * receiver, const char* slot) {
    return registerSequence(play, sequence, receiver, slot);
}

bool HotkeyManager::registerStop(QString sequence, QObject * receiver, const char* slot) {
    return registerSequence(stop, sequence, receiver, slot);
}
