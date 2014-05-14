#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QJsonObject * hash) :
  QDialog(parent), ui(new Ui::SettingsDialog) {
  ui -> setupUi(this);

  setWindowTitle("Settings ept");

  ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
  ui -> treeView -> setModel(new HotkeyModel(hash, this));
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

QJsonObject * SettingsDialog::getDefaults() {

}
