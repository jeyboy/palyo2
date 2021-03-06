#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QColorDialog>

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

private slots:
    void on_cancelButton_clicked();

    void on_acceptButton_clicked();

    void on_browseButton_clicked();

    void on_defaultColorButton_clicked();

    void on_listenedColorButton_clicked();

    void on_likedColorButton_clicked();

    void on_playedColorButton_clicked();

    void on_folderColorButton_clicked();

    void on_defaultItemTextColorButton_clicked();

    void on_selectedItemTextColorButton_clicked();

    void on_defaultItemInfoTextColorButton_clicked();

    void on_selectedItemInfoTextColorButton_clicked();

    void on_spectrumColor_clicked();

    void on_spectrumColor2_clicked();

    void on_spectrumColor3_clicked();

private:
    bool execColorDialog(QColor & color);

    Ui::SettingsDialog *ui;
    bool iconSizeChanged;
    QColor defaultColor, listenedColor, likedColor, playedColor, folderColor, spectrumColor, spectrumColor2, spectrumColor3;
    QColor itemTextColor, selectedItemTextColor, itemInfoTextColor, selectedItemInfoTextColor;
};


#endif // SETTINGS_DIALOG_H
