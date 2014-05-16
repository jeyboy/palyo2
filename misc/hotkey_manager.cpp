#include "hotkey_manager.h"

HotkeyManager *HotkeyManager::self = 0;

HotkeyManager *HotkeyManager::instance() {
    if(!self)
        self = new HotkeyManager();
    return self;
}

bool HotkeyManager::registerNext(QString sequence, QObject * receiver, char* slot) {
    next = new QxtGlobalShortcut(QKeySequence(sequence));
    if (next -> isEnabled()) {
        connect(next, SIGNAL(activated()), receiver, slot);
        return true;
    } else {
        return false;
    }
}

bool HotkeyManager::registerNextAndDelete(QString sequence, QObject * receiver, char* slot) {

}

bool HotkeyManager::registerPrev(QString sequence, QObject * receiver, char* slot) {

}

bool HotkeyManager::registerPlay(QString sequence, QObject * receiver, char* slot) {

}

bool HotkeyManager::registerStop(QString sequence, QObject * receiver, char* slot) {

}
