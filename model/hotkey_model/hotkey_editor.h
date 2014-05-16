#ifndef HOTKEY_EDITOR_H
#define HOTKEY_EDITOR_H

#include <QLineEdit>
#include <QKeyEvent>

class HotkeyEditor : public QLineEdit {
    Q_OBJECT

public:
    HotkeyEditor(QWidget *parent = 0);

signals:
    void editingFinished();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
};

#endif // HOTKEY_EDITOR_H
