#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "model/hotkey_model/hotkey_model.h"
#include "model/hotkey_model/hotkey_model_item.h"
#include "model/hotkey_model/hotkey_delegate.h"
#include "misc/settings.h"
#include "misc/hotkey_manager.h"
#include "media/player.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent);
    ~SettingsDialog();

    static void registerHotkeys(QWidget * receiver);

    bool isIconSizeChanged() const;
    bool isBigIcon() const;

private slots:
    void on_cancelButton_clicked();

    void on_acceptButton_clicked();

    void on_browseButton_clicked();

private:
    Ui::SettingsDialog *ui;
    bool iconSizeChanged;
};


#endif // SETTINGS_DIALOG_H
