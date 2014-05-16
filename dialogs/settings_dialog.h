#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include "model/hotkey_model/hotkey_model.h"
#include "model/hotkey_model/hotkey_model_item.h"
#include "model/hotkey_model/hotkey_delegate.h"
#include "misc/settings.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
};


#endif // SETTINGS_DIALOG_H
