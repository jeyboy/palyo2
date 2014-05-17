#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog) {
  ui -> setupUi(this);

  setWindowTitle("Settings ept");

  ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
  ui -> treeView -> setItemDelegate(new HotkeyDelegate(ui -> treeView));
  ui -> treeView -> setModel(new HotkeyModel(Settings::instance() -> getHotKeys(), this));
  ui -> treeView -> hideColumn(2);
  ui -> treeView -> setColumnWidth(0, 250);

  ui -> downloadPath -> setText(Settings::instance() -> getDownloadPath());
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::registerHotkeys(QWidget * receiver) {
    QList<HotkeyModelItem *> list = *Settings::instance() -> getHotKeys();

    foreach(HotkeyModelItem * key, list) {
        switch(key -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(nextItemTriggered()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(nextItemWithDelTriggered()));
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(prevItemTriggered()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(stop()));
            break;}
        }
    }
}


void SettingsDialog::on_cancelButton_clicked() {
    reject();
}

void SettingsDialog::on_acceptButton_clicked() {
    HotkeyModel * model = dynamic_cast<HotkeyModel *>(ui -> treeView -> model());
    Settings::instance() -> setHotKeys(model -> toplevelItems());

    Settings::instance() -> setDownloadPath(ui -> downloadPath -> text());
    accept();
}

void SettingsDialog::on_browseButton_clicked() {
    QString path = QFileDialog::getExistingDirectory(this, "Please choose new download path");
    if (!path.isEmpty()) {
        if (!path.endsWith('/'))
            path += "/";
        ui -> downloadPath -> setText(path);
    }
}
