#ifndef HOTKEY_EDITOR_H
#define HOTKEY_EDITOR_H

#include <QLineEdit>
#include <QKeyEvent>

#include "misc/hotkey_manager.h"

class HotkeyEditor : public QLineEdit {
    Q_OBJECT

public:
    HotkeyEditor(int hotkeyType, QWidget *parent = 0);

signals:
    void editingFinished();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:

    int keyType;
};

#endif // HOTKEY_EDITOR_H