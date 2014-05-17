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
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::registerHotkeys(QWidget * receiver) {
    QList<HotkeyModelItem *> list = *Settings::instance() -> getHotKeys();

    foreach(HotkeyModelItem * key, list) {
        switch(key -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::instance() -> registerNext(key -> data(1).toString(), receiver, SLOT(nextItemTriggered()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::instance() -> registerNextAndDelete(key -> data(1).toString(), receiver, SLOT(nextItemWithDelTriggered()));
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::instance() -> registerPrev(key -> data(1).toString(), receiver, SLOT(prevItemTriggered()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::instance() -> registerPlay(key -> data(1).toString(), Player::instance(), SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::instance() -> registerStop(key -> data(1).toString(), Player::instance(), SLOT(stop()));
            break;}
        }
    }
}
