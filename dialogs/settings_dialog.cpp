#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog) {
  ui -> setupUi(this);

  setWindowTitle("Settings ept");

  ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
  ui -> treeView -> setItemDelegate(new HotkeyDelegate(ui -> treeView));
  ui -> treeView -> setModel(new HotkeyModel(Settings::instance() -> getHotKeys(), this));
  ui -> treeView -> setColumnWidth(0, 250);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}
