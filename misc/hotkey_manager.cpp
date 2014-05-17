#include "hotkey_manager.h"
#include <qDebug>

HotkeyManager *HotkeyManager::self = 0;

HotkeyManager *HotkeyManager::instance() {
    if(!self)
        self = new HotkeyManager();
    return self;
}

QxtGlobalShortcut * HotkeyManager::registerSequence(QObject * receiver, const char* slot) {
    QxtGlobalShortcut * shortcut = new QxtGlobalShortcut();
    connect(shortcut, SIGNAL(activated()), receiver, slot);
    return shortcut;
}

bool HotkeyManager::updateSequence(QxtGlobalShortcut * shortcut, QKeySequence sequence) {
    QKeySequence oldSequence = shortcut -> shortcut();

    if (shortcut -> setShortcut(sequence)) {
        return true;
    } else {
        shortcut -> setShortcut(oldSequence);
    }

    return false;
}

bool HotkeyManager::registerSequence(int hotkeyType, QString sequence, QObject * receiver, const char* slot) {
    switch(hotkeyType) {
        case HOTKEY_NEXT: {
            if (next == 0) {
                next = registerSequence(receiver, slot);
                return next -> setShortcut(QKeySequence(sequence));
            } else  return updateSequence(next, QKeySequence(sequence));
        }
        case HOTKEY_NEXT_AND_DELETE: {
            if (next_and_delete == 0) {
                next_and_delete = registerSequence(receiver, slot);
                return next_and_delete -> setShortcut(QKeySequence(sequence));
            } else  return updateSequence(next, QKeySequence(sequence));
        }
        case HOTKEY_PREV: {
            if (prev == 0) {
                prev = registerSequence(receiver, slot);
                return prev -> setShortcut(QKeySequence(sequence));
            } else  return updateSequence(next, QKeySequence(sequence));
        }
        case HOTKEY_PLAY: {
            if (play == 0) {
                play = registerSequence(receiver, slot);
                return play -> setShortcut(QKeySequence(sequence));
            } else  return updateSequence(next, QKeySequence(sequence));
        }
        case HOTKEY_STOP: {
            if (stop == 0) {
                stop = registerSequence(receiver, slot);
                return stop -> setShortcut(QKeySequence(sequence));
            } else  return updateSequence(next, QKeySequence(sequence));
        }
    }

    return false;
}
