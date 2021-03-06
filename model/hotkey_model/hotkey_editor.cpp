#include "hotkey_editor.h"
#include "qDebug"

HotkeyEditor::HotkeyEditor(int hotkeyType, QWidget *parent) : QKeySequenceEdit(parent) {
    keyType = hotkeyType;
}

void HotkeyEditor::keyReleaseEvent(QKeyEvent *) {
    emit editingFinished();

    if (!HotkeyManager::instance() -> registerSequence(keyType, keySequence().toString())) {
        qDebug() << "LOL";
    }
}
