#include "tabdialog.h"
#include "ui_tabdialog.h"

CBHash TabDialog::VKSettings(int uid) {
    CBHash res;
    res.insert("d", 0);
    res.insert("i", 0);
    res.insert("p", 1);
    res.insert("t", 4);
    res.insert("uid", uid);

    return res;
}

TabDialog::TabDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::TabDialog) {
  ui->setupUi(this);

  setWindowTitle("Tab settings");
  ui -> interactive -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
}

TabDialog::~TabDialog() {
  delete ui;
}

QString TabDialog::getName() {
  return ui ->tabName ->text();
}

void TabDialog::setName(QString name) {
  ui -> tabName -> setText(name);
}

CBHash TabDialog::getSettings() {
  CBHash res;
  res.insert("d", ui -> deleteFile -> checkState() == Qt::Checked ? 1 : 0);
  res.insert("i", ui -> interactive -> checkState() == Qt::Checked ? 1 : 0);
  res.insert("p", ui -> playlist -> checkState() == Qt::Checked ? 1 : 0);

  if (ui -> isListRadio -> isChecked())
      res.insert("t", VIEW_LIST);
  else if (ui -> isOneLevelTreeRadio -> isChecked())
      res.insert("t", VIEW_LEVEL_TREE);
  else if (ui -> isTreeRadio -> isChecked())
      res.insert("t", VIEW_TREE);
  else if (ui -> isOneLevelTreeBreadcrumbRadio -> isChecked())
      res.insert("t", VIEW_LEVEL_TREE_BREADCRUMB);
  else res.insert("t", VIEW_VK);

  return res;
}

void TabDialog::setSettings(CBHash hash) {
  ui -> deleteFile -> setChecked(hash.value("d") == 1);
  ui -> interactive -> setChecked(hash.value("i") == 1);
  ui -> playlist -> setChecked(hash.value("p") == 1);

  ui -> isListRadio -> setChecked(hash.value("t") == VIEW_LIST);
  ui -> isOneLevelTreeRadio -> setChecked(hash.value("t") == VIEW_LEVEL_TREE);
  ui -> isOneLevelTreeBreadcrumbRadio -> setChecked(hash.value("t") == VIEW_LEVEL_TREE_BREADCRUMB);
  ui -> isVkRadio -> setChecked(hash.value("t") == VIEW_VK);
  ui -> isTreeRadio -> setChecked(hash.value("t") == VIEW_TREE);

  ui -> isListRadio -> setEnabled(false);
  ui -> isTreeRadio -> setEnabled(false);
  ui -> isOneLevelTreeRadio -> setEnabled(false);
  ui -> isOneLevelTreeBreadcrumbRadio -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
}
